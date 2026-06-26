#include "sender.h"
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

bool NetworkSender::DispatchPayload(const std::string& filePath, const std::string& receiverIP) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open file: " << filePath << std::endl;
        return false;
    }
    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    int clientSock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(receiverIP.c_str());
    server.sin_port = htons(8080);

    if (connect(clientSock, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Error: Cannot connect to " << receiverIP << std::endl;
#ifdef _WIN32
        closesocket(clientSock);
        WSACleanup();
#else
        close(clientSock);
#endif
        return false;
    }

    long long size = (long long)data.size();
    send(clientSock, (char*)&size, sizeof(long long), 0);

    long long totalSent = 0;
    while (totalSent < size) {
        int chunk = (int)std::min((long long)4096, size - totalSent);
        int sent = send(clientSock, (char*)data.data() + totalSent, chunk, 0);
        if (sent <= 0) break;
        totalSent += sent;
    }
    std::cout << "Sent: " << totalSent << " bytes to " << receiverIP << std::endl;

#ifdef _WIN32
    closesocket(clientSock);
    WSACleanup();
#else
    close(clientSock);
#endif
    return true;
}