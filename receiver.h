#pragma once
#include <string>

class NetworkReceiver {
public:
    bool ListenAndDecryptPayload(const std::string& password, const std::string& outputFile);
};