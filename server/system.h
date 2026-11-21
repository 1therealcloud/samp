#ifndef SAMPSRV_SYSTEM_H
#define SAMPSRV_SYSTEM_H

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>

typedef int SOCKET;
typedef int BOOL;
#endif

#endif
