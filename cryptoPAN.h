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
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <string>

using namespace CryptoPP;

class CryptoPan {
public:
    CryptoPan(const std::string& key);
    ECB_Mode<CryptoPP::AES>::Encryption aes;
    std::array<uint8_t, 16> pad;
    std::vector<std::pair<uint32_t, uint32_t>> masks;

    uint32_t toInt(const std::array<uint8_t, 4>& arr);

};

#endif // CRYPTOPAN_H