#include <iostream>
#include <string>
#include <iomanip>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>


#include "cryptoPAN.h"

int main() {
    using namespace CryptoPP;

    std::cout << "main started" << std::endl;
    std::string key = "boojahyoo3vaeToong0Eijee7Ahz3yee";

    CryptoPan *cp = new CryptoPan(key);
    cout << cp->anonymize("boojahyoo3vaeToong0Eijee7Ahz3yee","192.0.2.1") << endl;

    delete cp;

    return 0;
}