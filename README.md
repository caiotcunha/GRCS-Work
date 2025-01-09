# GRCS-Work
Trabalho relacionado à disciplina de gerenciamento de redes e computadores.
Implementação de criptoPAN baseado na implementaçãop do pycryptopan.

https://github.com/FFM/pycryptopan/blob/master/cryptopan.py
https://metacpan.org/dist/IP-Anonymous/source/lib/IP/Anonymous.pm


# Usage
O código python pode ser executado com python3 cryptopan.py

Para rodar o código c++ antes é necessário compilar a biblioteca cripto++
Para isso use o comando:
```
cd cryptopp
make
sudo make install
```

Depois volte para a pasta do arquivo main.cpp e compile com o comando:
```
g++ -I/usr/local/include/cryptopp -L/usr/local/lib -lcryptopp -o main main.cpp cryptoPAN.cpp
```

Essa sequência funciona em ambientes linux, no caso de windows devem ser feitas as devidas alterações.

