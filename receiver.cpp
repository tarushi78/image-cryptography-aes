#include "receiver.h"
#include "aes.h"
#include <iostream>
#include <fstream>
#include <vector>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

bool NetworkReceiver::ListenAndDecryptPayload(const std::string& password, const std::string& outputFile) {
    std::vector<uint8_t> key = DeriveKey(password);
    std::vector<uint8_t> iv = DeriveIV(password);

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    if (bind(serverSock, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Error: Port 8080 bind failed." << std::endl;
        return false;
    }
    listen(serverSock, 1);
    std::cout << "Listening on port 8080..." << std::endl;

    int clientSock = accept(serverSock, NULL, NULL);
    std::cout << "Sender connected." << std::endl;

    long long fileSize = 0;
    recv(clientSock, (char*)&fileSize, sizeof(long long), 0);

    std::vector<uint8_t> received;
    char buffer[4096];
    int bytes;
    while ((long long)received.size() < fileSize) {
        bytes = recv(clientSock, buffer, 4096, 0);
        if (bytes <= 0) break;
        received.insert(received.end(), buffer, buffer + bytes);
    }

    std::vector<uint8_t> decrypted = AES_Decrypt(received, key, iv);

    std::ofstream ostream(outputFile, std::ios::binary);
    ostream.write((char*)decrypted.data(), decrypted.size());
    std::cout << "Decrypted image saved: " << outputFile << std::endl;

#ifdef _WIN32
    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();
#else
    close(clientSock);
    close(serverSock);
#endif
    return true;
}