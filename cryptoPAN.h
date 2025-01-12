// cryptoPAN.h
#ifndef CRYPTOPAN_H
#define CRYPTOPAN_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <string>
#include <stdexcept>

using namespace std;
using namespace CryptoPP;

class CryptoPan {
public:
    CryptoPan(const std::string& key);
    ECB_Mode<CryptoPP::AES>::Encryption aes;
    std::array<uint8_t, 16> pad;
    std::vector<std::pair<uint32_t, uint32_t>> masks;
    std::array<uint8_t, 4> toArray(uint32_t n);
    uint8_t calc(uint32_t a);
    uint32_t toInt(const std::array<uint8_t, 4>& arr);
    string anonymize(const string& key, const string& ip);
};



class CryptoPanError : public std::exception {
private:
    std::string message;  

public:
    explicit CryptoPanError(const std::string& msg);

    const char* what() const noexcept override;
};


#endif // CRYPTOPAN_H