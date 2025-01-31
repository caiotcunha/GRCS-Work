#include <vector>
#include <sstream>
#include <unordered_map>
#include "Tcrypto.h"

using namespace std;
using namespace CryptoPP;

TCryptoError::TCryptoError(const std::string& msg) : message(msg) {}

const char* TCryptoError::what() const noexcept {
    return message.c_str();
}

uint32_t TCrypto::toInt(const std::array<uint8_t, 4>& arr) {
    return (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
}

TCrypto::TCrypto(const std::string& key, uint8_t kBits) : k(kBits) {
    if (key.size() != 32) {
        throw TCryptoError("Key must be a 32 byte long string");
    }

    CryptoPP::SecByteBlock aesKey(reinterpret_cast<const uint8_t*>(key.data()), 16);
    aes.SetKey(aesKey, aesKey.size());

    std::memcpy(pad.data(), key.data() + 16, 16);
    aes.ProcessData(pad.data(), pad.data(), 16);

    std::array<uint8_t, 4> f4;
    std::copy(pad.begin(), pad.begin() + 4, f4.begin());
    uint32_t f4bp = toInt(f4);

    for (int p = 0; p < 32; ++p) {
        uint32_t mask = (p == 0) ? 0 : (0xFFFFFFFF >> (32 - p)) << (32 - p);
        uint32_t complement = f4bp & (~mask);
        masks.emplace_back(mask, complement);
    }
}

std::array<uint8_t, 4> TCrypto::toArray(uint32_t n) {
    return {static_cast<uint8_t>((n >> 24) & 0xFF),
            static_cast<uint8_t>((n >> 16) & 0xFF),
            static_cast<uint8_t>((n >> 8) & 0xFF),
            static_cast<uint8_t>(n & 0xFF)};
}

uint8_t TCrypto::calc(uint32_t a) {
    std::array<uint8_t, 4> a_array = toArray(a);

    vector<uint8_t> inp(a_array.begin(), a_array.end());
    inp.insert(inp.end(), pad.begin() + 4, pad.end());

    vector<uint8_t> aes_output(CryptoPP::AES::BLOCKSIZE);
    aes.ProcessData(aes_output.data(), inp.data(), inp.size());

    uint8_t out = aes_output[0];
    return (out >> 7) & 0x01;
}

std::string TCrypto::anonymize(const std::string& ip) {
    vector<uint8_t> address;
    stringstream ss(ip);
    string segment;

    while (getline(ss, segment, '.')) {
        address.push_back(static_cast<uint8_t>(stoi(segment)));
    }

    if (address.size() != 4) {
        throw TCryptoError("Invalid IPv4 Address");
    }

    uint32_t ipAddress = toInt({address[0], address[1], address[2], address[3]});

    uint32_t prefixMask = (0xFFFFFFFF << (32 - k)) & 0xFFFFFFFF;
    uint32_t prefix = ipAddress & prefixMask;

   
    if (prefixMap.find(prefix) != prefixMap.end()) {
        return prefixMap[prefix];
    }

    uint32_t newAddress = ipAddress;
    vector<uint32_t> addresses;

    for (const auto& mask : masks) {
        uint32_t masked = (newAddress & mask.first) | mask.second;
        addresses.push_back(masked);
    }

    uint32_t result = 0;
    for (const auto& masked : addresses) {
        result = (result << 1) | calc(masked);
    }

    result = result ^ newAddress;

    std::array<uint8_t, 4> finalArray = toArray(result);
    stringstream ipStream;
    for (size_t i = 0; i < finalArray.size(); ++i) {
        if (i > 0) ipStream << ".";
        ipStream << static_cast<int>(finalArray[i]);
    }

    // Armazenar no mapa de prefixos
    string anonymizedIp = ipStream.str();
    prefixMap[prefix] = anonymizedIp;

    return anonymizedIp;
}
