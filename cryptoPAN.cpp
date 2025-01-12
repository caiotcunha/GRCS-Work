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

    // Configurar AES com os primeiros 16 bytes da chave
    CryptoPP::SecByteBlock aesKey(reinterpret_cast<const uint8_t*>(key.data()), 16);
    aes.SetKey(aesKey, aesKey.size());


    // Gerar o padding usando os últimos 16 bytes da chave
    std::memcpy(pad.data(), key.data() + 16, 16);

    // Guardar processado
    std::array<uint8_t, 16> processed;
    aes.ProcessData(processed.data(), pad.data(),16);

    // for (int i = 0; i < 16; ++i) {
    //     std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)test[i];
    // }

    // Obter os primeiros 4 bytes de "pad" e convertê-los em inteiro
    std::array<uint8_t, 4> f4;
    std::copy(processed.begin(), processed.begin() + 4, f4.begin());

    // for (int i = 0; i < 4; ++i) {
    //     std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)f4[i] << "\n";
    // }

    uint32_t f4bp = toInt(f4);

    //std::cout << "f4bp: " << f4bp << std::endl;

    // Gerar máscaras e complementos
    for (int p = 0; p < 32; ++p) {
        uint32_t mask = (0xFFFFFFFF >> (32 - p)) << (32 - p);
        uint32_t complement = f4bp & (~mask);
        masks.emplace_back(mask, complement);
    }

    //for (int i = 0; i < 32; ++i) {
       // std::cout << std::hex << std::setw(8) << std::setfill('0') << masks[i].first << " " << masks[i].second << "\n";
    //}

}

std::array<uint8_t, 4> CryptoPan::toArray(uint32_t n) {
    std::array<uint8_t, 4> result;

    for (int i = 3; i >= 0; --i) {
        result[3 - i] = (n >> (i * 8)) & 0xFF;  
    }

    return result;
}

uint8_t CryptoPan::calc(uint32_t a) {
        std::array<uint8_t, 4> a_array = toArray(a);
        
        cout << endl;
        std::vector<uint8_t> inp(a_array.begin(), a_array.end());

        inp.insert(inp.end(), pad.begin() + 4, pad.end());

        std::vector<uint8_t> aes_output(CryptoPP::AES::BLOCKSIZE);
        aes.ProcessData(aes_output.data(), inp.data(), inp.size());

        uint8_t out = aes_output[0];

        return (out >> 7) & 0x01;
}


std::string CryptoPan::anonymize(const string& key, const string& ip) {
    uint32_t a = 123456;  

    uint8_t neww = calc(a);
    cout << neww << endl;

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
    
    std::vector<uint32_t> maskedAddresses;
        for (const auto& mask : masks) {
            uint32_t masked = (newAddress & mask.first) | mask.second;
            maskedAddresses.push_back(masked);
        }
   
    uint32_t result = 0;
    for (const auto& masked : maskedAddresses) {
        result = (result << 1) | calc(masked);
    }
    
    result = result ^ newAddress;

    
    std::array<uint8_t, 4> finalArray = toArray(result);


    std::stringstream ipStream;
    for (size_t i = 0; i < finalArray.size(); ++i) {
        if (i > 0) ipStream << ".";  
        ipStream << static_cast<int>(finalArray[i]);
    }

    return ipStream.str();

}

