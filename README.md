# GRCS-Work
Trabalho relacionado à disciplina de gerenciamento de redes e computadores.
Implementação de criptoPAN baseado na implementaçãop do pycryptopan.

https://github.com/FFM/pycryptopan/blob/master/cryptopan.py
https://metacpan.org/dist/IP-Anonymous/source/lib/IP/Anonymous.pm


# Usage
O código python pode ser executado com python3 cryptopan.py

Para rodar o código c++ antes é necessário instalar biblioteca cripto++

Para isso use o comando:
```
sudo apt update
sudo apt install libcrypto++-dev
sudo apt install libcrypto++-utils
```

Verifica a instalação com o comando:
```
dpkg -s libcrypto++-dev
```

Para compilar o código use o comando:
```
g++ -g main.cpp cryptoPAN.cpp -o main -lcryptopp
```

Essa sequência funciona em ambientes linux, no caso de windows devem ser feitas as devidas alterações.

# Teste
Estou desenvolvendo o teste de forma que os dois códigos inicialmente sejam iguais