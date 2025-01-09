// Implementação em C++ do algoritmo cripto-PAN


#include "cryptoPAN.h"
#include <iostream>
#include <aes.h>
#include <modes.h>
#include <hex.h>

using namespace CryptoPP;


CriptoPan::CriptoPan(const unsigned char* key, size_t keySize) {

    ECB_Mode<AES>::Encryption ecbEncryption;
    ecbEncryption.SetKey(key, keySize);

}