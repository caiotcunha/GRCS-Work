// Implementação em C++ do algoritmo cripto-PAN


#include "cryptoPAN.h"

using namespace CryptoPP;

uint32_t CryptoPan::toInt(const std::array<uint8_t, 4>& arr) {
    return (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
}

CryptoPan::CryptoPan(const std::string& key) {

    // Configurar AES com os primeiros 16 bytes da chave
    CryptoPP::SecByteBlock aesKey(reinterpret_cast<const uint8_t*>(key.data()), 16);
    aes.SetKey(aesKey, aesKey.size());

    // Gerar o padding usando os últimos 16 bytes da chave
    std::array<uint8_t, 16> last16;
    std::memcpy(last16.data(), key.data() + 16, 16);
    aes.ProcessData(last16.data(), pad.data(),16);

    // Obter os primeiros 4 bytes de "pad" e convertê-los em inteiro
    std::array<uint8_t, 4> f4;
    std::copy(pad.begin(), pad.begin() + 4, f4.begin());
    uint32_t f4bp = toInt(f4);

    std::cout << "f4bp: " << std::hex << f4bp << std::endl;

        // Gerar máscaras e complementos
        // for (int p = 0; p < 32; ++p) {
        //     uint32_t mask = (0xFFFFFFFF >> (32 - p)) << (32 - p);
        //     uint32_t complement = f4bp & (~mask);
        //     masks.emplace_back(mask, complement);
        // }

    
}