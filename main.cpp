#include <iostream>
#include <string>
#include <iomanip>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>


//#include "cryptoPAN.h"
//#include "Tcrypto.h"
#include "DSPAN.h"

int main() {
    using namespace CryptoPP;

    std::cout << "main started" << std::endl;
    std::string key = "boojahyoo3vaeToong0Eijee7Ahz3yee";

    //CryptoPan *cp = new CryptoPan(key);
    //cout << cp->anonymize("boojahyoo3vaeToong0Eijee7Ahz3yee","192.0.2.1") << endl;
    //cout << cp->anonymize("boojahyoo3vaeToong0Eijee7Ahz3yee","192.0.2.2") << endl;
    //delete cp;

    TCrypto *tcp = new TCrypto(key,1);
    cout << tcp->anonymize("192.0.2.1") << endl;
    cout << tcp->anonymize("192.0.2.25") << endl;
    return 0;
}