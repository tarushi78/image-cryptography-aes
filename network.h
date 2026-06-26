#pragma once
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

class NetworkSocket {
public:
    static bool InitializeSockets();
    static void CloseSocket(int socketDescriptor);
    static void CleanupSockets();
};