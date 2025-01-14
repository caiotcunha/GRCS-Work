// Implementação em C++ do algoritmo cripto-PAN

#include<vector>
#include <sstream>
#include "cryptoPAN.h"
using namespace std;
using namespace CryptoPP;

CryptoPanError::CryptoPanError(const std::string& msg) : message(msg) {}

const char* CryptoPanError::what() const noexcept {
    return message.c_str();
}

uint32_t CryptoPan::toInt(const std::array<uint8_t, 4>& arr) {
    return (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
}

CryptoPan::CryptoPan(const std::string& key) {

    // case of error
    if (key.size() != 32) {
        throw CryptoPanError("Key must be a 32 byte long string");
    }

    // Configurar AES com os primeiros 16 bytes da chave
    CryptoPP::SecByteBlock aesKey(reinterpret_cast<const uint8_t*>(key.data()), 16);
    aes.SetKey(aesKey, aesKey.size());


    // Gerar o padding usando os últimos 16 bytes da chave
    std::memcpy(pad.data(), key.data() + 16, 16);

    // Guardar processado
    aes.ProcessData(pad.data(), pad.data(),16);

    // Obter os primeiros 4 bytes de "pad" e convertê-los em inteiro
    std::array<uint8_t, 4> f4;
    std::copy(pad.begin(), pad.begin() + 4, f4.begin());

    uint32_t f4bp = toInt(f4);

    // Gerar máscaras e complementos
    for (int p = 0; p < 32; ++p) {
        uint32_t mask = (p == 0) ? 0 : (0xFFFFFFFF >> (32 - p)) << (32 - p);
        uint32_t complement = f4bp & (~mask);
        masks.emplace_back(mask, complement);
    }

}

array<uint8_t, 4> CryptoPan::toArray(uint32_t n) {
    return {static_cast<uint8_t>((n >> 24) & 0xFF),
            static_cast<uint8_t>((n >> 16) & 0xFF),
            static_cast<uint8_t>((n >> 8) & 0xFF),
            static_cast<uint8_t>(n & 0xFF)};
}

uint8_t CryptoPan::calc(uint32_t a) {
        array<uint8_t, 4> a_array = toArray(a);

        vector<uint8_t> inp(a_array.begin(), a_array.end());

        inp.insert(inp.end(), pad.begin() + 4, pad.end());

        vector<uint8_t> aes_output(CryptoPP::AES::BLOCKSIZE);
        aes.ProcessData(aes_output.data(), inp.data(), inp.size());

        uint8_t out = aes_output[0];

        return (out >> 7) & 0x01;
}


std::string CryptoPan::anonymize(const string& key, const string& ip) {
    vector<uint8_t> address;
    stringstream ss(ip);
    string segment;

    while (getline(ss, segment, '.')) {
        address.push_back(static_cast<uint8_t>(stoi(segment))); 
    }

    if (address.size() != 4) {
        throw CryptoPanError("Invalid IPv4 Address");
    }

    uint32_t newAddress = toInt({address[0], address[1], address[2], address[3]});

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
    
    array<uint8_t, 4> finalArray = toArray(result);


    stringstream ipStream;
    for (size_t i = 0; i < finalArray.size(); ++i) {
        if (i > 0) ipStream << ".";  
        ipStream << static_cast<int>(finalArray[i]);
    }

    return ipStream.str();

}

