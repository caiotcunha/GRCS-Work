#include <iostream>
#include <string>
#include <iomanip>
#include <crypto++/sha.h>
#include <crypto++/hex.h>


#include "cryptoPAN.h"

int main() {
    using namespace CryptoPP;

    std::cout << "main started" << std::endl;
    std::string key = "boojahyoo3vaeToong0Eijee7Ahz3yee";

    CryptoPan *cp = new CryptoPan(key);

    delete cp;

    return 0;
}