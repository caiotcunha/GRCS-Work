#include <iostream>
#include <string>
#include <iomanip>  // Para manipulação de saída
#include <aes.h>
#include <modes.h>
#include <hex.h>

#include "cryptoPAN.h"

using namespace CryptoPP;

int main() {
    std::cout << "main started" << std::endl;
    byte key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x97, 0x75, 0x46, 0x20, 0x60, 0x01 };

    CriptoPan *cp = new CriptoPan(key, 16);

    delete cp;


    return 0;
}