#include "Encrypt.h"
#include "aes.h"
#include <iostream>
#include <fstream>
#include <vector>

ImageEncryptor::ImageEncryptor(const std::string& password) {
    key = DeriveKey(password);
    iv = DeriveIV(password);
}

bool ImageEncryptor::EncryptImage(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream file(inputFile, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open input file: " << inputFile << std::endl;
        return false;
    }
    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    std::vector<uint8_t> encrypted = AES_Encrypt(data, key, iv);
    std::ofstream outfile(outputFile, std::ios::binary);
    outfile.write((char*)encrypted.data(), encrypted.size());
    std::cout << "Encryption complete: " << outputFile << std::endl;
    return true;
}

bool ImageEncryptor::DecryptImage(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream file(inputFile, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open encrypted file: " << inputFile << std::endl;
        return false;
    }
    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    std::vector<uint8_t> decrypted = AES_Decrypt(data, key, iv);
    std::ofstream outfile(outputFile, std::ios::binary);
    outfile.write((char*)decrypted.data(), decrypted.size());
    std::cout << "Decryption complete: " << outputFile << std::endl;
    return true;
}