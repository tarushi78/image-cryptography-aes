#pragma once
#include <string>

class NetworkSender {
public:
    bool DispatchPayload(const std::string& filePath, const std::string& receiverIP);
};