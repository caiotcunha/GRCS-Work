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

    // for (int i = 0; i < 32; ++i) {
    //     std::cout << std::hex << std::setw(8) << std::setfill('0') << masks[i].first << " " << masks[i].second << "\n";
    // }

}

// Implementar a função de anonimização
// CryptoPan::anonymize(const std::string& key){

// }