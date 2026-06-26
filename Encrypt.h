#pragma once
#include <string>
#include <vector>
#include <cstdint>

class ImageEncryptor {
private:
    std::vector<uint8_t> key;
    std::vector<uint8_t> iv;
public:
    ImageEncryptor(const std::string& password);
    bool EncryptImage(const std::string& inputFile, const std::string& outputFile);
    bool DecryptImage(const std::string& inputFile, const std::string& outputFile);
};