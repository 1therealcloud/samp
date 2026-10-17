#include "cmdwindow.h"
#include "BitStream.h"
#include "RakClientInterface.h"
#include <string.h>

// samp.dll RVA: 0x002aca24
extern void *g_pNetGame;

// Partial view: only the independently observed client field is described.
#pragma pack(push, 1)
struct InputNetGameView {
    unsigned char unknown[0x2c];
    RakClientInterface *client;
};
#pragma pack(pop)
typedef char InputClientOffset[(offsetof(InputNetGameView, client) == 0x2c) ? 1 : -1];

// Native 0x1011c310 contains the 32-bit numeric RPC identifier 50.
// samp.dll RVA: 0x0011c310
int g_inputCommandRpcId = 50;

// Recovered from 0x10069340. The input length excludes the trailing NUL.
// samp.dll RVA: 0x00069340
void CInput::Send(const char *text) {
    if (g_pNetGame) {
        RakNet::BitStream stream;
        int length = (int)strlen(text);
        stream.Write(length);
        stream.Write(text, length);
        InputNetGameView *netGame = (InputNetGameView *)g_pNetGame;
        netGame->client->RPC(&g_inputCommandRpcId, &stream,
            HIGH_PRIORITY, RELIABLE, 0, false);
    }
}
