#ifndef SAMPSRV_QUERY_H
#define SAMPSRV_QUERY_H

#include "system.h"

extern BOOL bRconSocketReply;
int ProcessQueryPacket(unsigned int binaryAddress, unsigned short port, char* data, int length, SOCKET socket);
void RconSocketReply(const char* message);

#endif
