#include "query.h"
#include "console.h"
#include "netgame.h"

#include <string.h>
#include <string>

namespace
{
const int QueryHeaderSize = 11;
const int MaxUdpPayload = 65507;

class RconReplyContext
{
public:
    RconReplyContext(SOCKET socket, const sockaddr_in& destination, const char* request);
    ~RconReplyContext();

    SOCKET socket;
    sockaddr_in destination;
    char header[QueryHeaderSize];

private:
    RconReplyContext(const RconReplyContext&);
    RconReplyContext& operator=(const RconReplyContext&);
    const RconReplyContext* previous;
};

const RconReplyContext* rconReply;

RconReplyContext::RconReplyContext(SOCKET socket, const sockaddr_in& destination, const char* request)
    : socket(socket), destination(destination), previous(rconReply)
{
    memcpy(header, request, sizeof(header));
    rconReply = this;
    bRconSocketReply = 1;
}

RconReplyContext::~RconReplyContext()
{
    rconReply = previous;
    bRconSocketReply = rconReply != NULL;
}

bool ReadRconString(const char*& cursor, const char* end, std::string& value)
{
    if (end - cursor < 2)
        return false;

    const unsigned int length = static_cast<unsigned char>(cursor[0]) |
                                (static_cast<unsigned int>(static_cast<unsigned char>(cursor[1])) << 8);
    cursor += 2;
    if (end - cursor < static_cast<int>(length) || memchr(cursor, '\0', length))
        return false;

    value.assign(cursor, length);
    cursor += length;
    return true;
}

void ProcessRconPacket(SOCKET socket, const sockaddr_in& destination, const char* data, int length)
{
    if (!pConsole->GetBoolVariable("rcon"))
        return;

    const char* cursor = data + QueryHeaderSize;
    const char* end = data + length;
    std::string password;
    if (!ReadRconString(cursor, end, password))
        return;

    const char* configuredPassword = pConsole->GetStringVariable("rcon_password");
    if (!configuredPassword || !*configuredPassword || password != configuredPassword)
    {
        logprintf("BAD RCON ATTEMPT BY: %s", inet_ntoa(destination.sin_addr));
        RconReplyContext reply(socket, destination, data);
        RconSocketReply("Invalid RCON password.");
        return;
    }

    std::string command;
    if (ReadRconString(cursor, end, command))
    {
        RconReplyContext reply(socket, destination, data);
        pConsole->Execute(command.c_str());
    }
}

void WriteQueryWord(char*& cursor, unsigned int value)
{
    *cursor++ = static_cast<char>(value & 0xff);
    *cursor++ = static_cast<char>((value >> 8) & 0xff);
}

void WriteQueryDword(char*& cursor, unsigned int value)
{
    WriteQueryWord(cursor, value);
    WriteQueryWord(cursor, value >> 16);
}

void WriteQueryString(char*& cursor, const char* value, unsigned int limit)
{
    unsigned int length = value ? static_cast<unsigned int>(strlen(value)) : 0;
    if (length > limit)
        length = limit;
    WriteQueryWord(cursor, length);
    WriteQueryWord(cursor, 0);
    if (length)
    {
        memcpy(cursor, value, length);
        cursor += length;
    }
}
}

// 0x0051c1a0 (Windows), 0x081d8a90 (Linux)
BOOL bRconSocketReply = 0;

// 0x0049e3a0 (Windows), 0x080de870 (Linux)
void RconSocketReply(const char* message)
{
    if (!rconReply || !message)
        return;

    const size_t length = strlen(message);
    if (length > MaxUdpPayload - QueryHeaderSize - 2)
        return;

    std::string response(rconReply->header, sizeof(rconReply->header));
    char lengthBytes[2];
    char* cursor = lengthBytes;
    WriteQueryWord(cursor, static_cast<unsigned int>(length));
    response.append(lengthBytes, sizeof(lengthBytes));
    response.append(message, length);
    sendto(rconReply->socket, response.data(), static_cast<int>(response.size()), 0,
           reinterpret_cast<const sockaddr*>(&rconReply->destination), sizeof(rconReply->destination));
}

// 0x0049e480 (Windows), 0x080de960 (Linux)
int ProcessQueryPacket(unsigned int binaryAddress, unsigned short port, char* data, int length, SOCKET socket)
{
    if (!data || length <= 4 || memcmp(data, "SAMP", 4) != 0 || !pConsole)
        return 0;
    if (length < QueryHeaderSize)
        return 1;

    sockaddr_in to = {0};
    to.sin_family = AF_INET;
    to.sin_port = htons(port);
    to.sin_addr.s_addr = binaryAddress;
    if (pConsole->GetBoolVariable("logqueries"))
        logprintf("[query:%c] from %s", data[10], inet_ntoa(to.sin_addr));

    if (data[10] == 'p')
    {
        if (length == 15)
            sendto(socket, data, length, 0, reinterpret_cast<const sockaddr*>(&to), sizeof(to));
        return 1;
    }
    if (!pConsole->GetBoolVariable("query"))
        return 1;

    switch (data[10])
    {
        case 'x':
            ProcessRconPacket(socket, to, data, length);
            break;
        case 'i':
        {
            char response[28 + 50 + 30 + 30];
            memcpy(response, data, QueryHeaderSize);
            char* cursor = response + QueryHeaderSize;
            const char* password = pConsole->GetStringVariable("password");
            *cursor++ = password && *password ? 1 : 0;
            WriteQueryWord(cursor, pNetGame ? pNetGame->GetPlayerPool().GetCount() : 0);
            WriteQueryWord(cursor, pConsole->GetIntVariable("maxplayers"));
            WriteQueryString(cursor, pConsole->GetStringVariable("hostname"), 50);
            WriteQueryString(cursor, pConsole->GetStringVariable("gamemodetext"), 30);
            WriteQueryString(cursor, pConsole->GetStringVariable("language"), 30);
            sendto(socket, response, static_cast<int>(cursor - response), 0,
                   reinterpret_cast<const sockaddr*>(&to), sizeof(to));
            break;
        }
        case 'r':
            pConsole->SendRules(socket, data, &to, sizeof(to));
            break;
        case 'c':
        case 'd':
        {
            const CPlayerPool* players = pNetGame ? &pNetGame->GetPlayerPool() : NULL;
            const unsigned short count = players ? players->GetCount() : 0;
            if (count > 100)
                break;

            char response[QueryHeaderSize + 2 + 100 * (CPlayerPool::MaxNameLength + 10)];
            memcpy(response, data, QueryHeaderSize);
            char* cursor = response + QueryHeaderSize;
            WriteQueryWord(cursor, count);
            const bool detailed = data[10] == 'd';
            for (int i = 0; players && i < CPlayerPool::MaxPlayers; ++i)
            {
                const CPlayerPool::Slot* player = players->GetAt(i);
                if (!player)
                    continue;
                if (detailed)
                    *cursor++ = static_cast<char>(i);
                const size_t nameLength = strlen(player->name);
                *cursor++ = static_cast<char>(nameLength);
                memcpy(cursor, player->name, nameLength);
                cursor += nameLength;
                WriteQueryDword(cursor, 0);
                if (detailed)
                    WriteQueryDword(cursor, pNetGame->GetPlayerPing(player->address));
            }
            sendto(socket, response, static_cast<int>(cursor - response), 0,
                   reinterpret_cast<const sockaddr*>(&to), sizeof(to));
            break;
        }
    }
    return 1;
}
