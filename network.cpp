#include "network.h"
#include <iostream>

bool NetworkSocket::InitializeSockets() {
#ifdef _WIN32
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
#else
    return true;
#endif
}

void NetworkSocket::CloseSocket(int socketDescriptor) {
#ifdef _WIN32
    closesocket(socketDescriptor);
#else
    close(socketDescriptor);
#endif
}

void NetworkSocket::CleanupSockets() {
#ifdef _WIN32
    WSACleanup();
#endif
}