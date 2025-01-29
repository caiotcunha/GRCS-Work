#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <time.h>

//#include "cryptoPAN.h"
#include "Tcrypto.h"
//#include "DSPAN.h"

double time1, timedif;

int main() {
    time1 = (double) clock();
    time1 = time1 / CLOCKS_PER_SEC; 
    using namespace CryptoPP;
    using namespace std;

    cout << "main started" << endl;
    string key = "boojahyoo3vaeToong0Eijee7Ahz3yee";

    string line;
    ifstream file("./database/ipv4.txt");

    //CryptoPan *cp = new CryptoPan(key);
    TCrypto *tcp = new TCrypto(key,24);
    //DSPAN *dscp = new DSPAN(key,24);

    if (file.is_open()){
        while (getline(file,line)){
            //cout << cp->anonymize("boojahyoo3vaeToong0Eijee7Ahz3yee",line) << endl;
            tcp->anonymize(line);
            //dscp->anonymize(line);
        }
        file.close();
    }

    //delete cp;
    delete tcp;

    timedif = ( ((double) clock()) / CLOCKS_PER_SEC) - time1;

    return 0;
}