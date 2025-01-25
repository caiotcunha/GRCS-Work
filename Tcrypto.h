#ifndef TCRYPTO_H
#define TCRYPTO_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <unordered_map>
#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <string>
#include <stdexcept>

using namespace std;
using namespace CryptoPP;

class TCrypto {
public:
    // Construtor atualizado para aceitar k (quantidade de bits do prefixo)
    TCrypto(const std::string& key, uint8_t kBits);

    // Funções auxiliares e atributos do algoritmo
    std::array<uint8_t, 4> toArray(uint32_t n);
    uint8_t calc(uint32_t a);
    uint32_t toInt(const std::array<uint8_t, 4>& arr);

    // Método para anonimizar um IP com base na configuração
    string anonymize(const std::string& ip);

private:
    // Quantidade de bits do prefixo para hash
    uint8_t k;

    // Configuração AES
    ECB_Mode<CryptoPP::AES>::Encryption aes;

    // Padding e máscaras
    std::array<uint8_t, 16> pad;
    std::vector<std::pair<uint32_t, uint32_t>> masks;

    // Memória hash para prefixos já processados
    std::unordered_map<uint32_t, std::string> prefixMap;
};

// Classe de erro para tratar exceções específicas
class TCryptoError : public std::exception {
private:
    std::string message;

public:
    explicit TCryptoError(const std::string& msg);

    const char* what() const noexcept override;
};

#endif // TCRYPTO_H
