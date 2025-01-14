#!/usr/bin/env ipython3

# Versão modificada do código presente em https://github.com/FFM/pycryptopan/blob/master/cryptopan.py
# Esse software é livre e pode ser modificado, sem fins lucrativos, conforme a licença presente no repositório original

from functools import reduce
# from pycrypto module
from Crypto.Cipher.AES import new as AES
# from Crypto import Random


class CryptoPanError(Exception):
    def __init__(self, value):
        self.value=value

    def __str__(self):
        return repr(self.value)


class CryptoPan():
    def __init__(self, key):
        if len(key)!=32:
            raise CryptoPanError("Key must be a 32 byte long string")
        self.aes=AES(key[0:16],1)
        self.pad=self.aes.encrypt(key[16:32])

        f4=self.pad[0:4]

        # Python 2 requires explicit conversion to ints
        if isinstance(f4, str):
            f4=[ord(x) for x in f4]
        
        f4bp=self.toint(f4)
        self.masks=[(mask, f4bp & (~ mask)) for mask in (0xFFFFFFFF >> (32 - p) << (32 - p) for p in range(0, 32))]

    def toint(self, array):
        return array[0] << 24 | array[1] <<16 | array[2] << 8 | array[3]

    def toarray(self, n):
        for i in range(3, -1, -1):
            yield (n >> (i * 8)) & 0xFF

    def anonymize(self, ip):
        result=0
        address=[int(x) for x in ip.split(".")]
        if len(address)!=4:
            raise CryptoPanError("Invalid IPv4 Address")

        address=self.toint(address)

        def calc(a):
            """ calculate the first bit for Crypto-PAN"""
            a_array = self.toarray(a)
                
            # Python 2 requires converting ints to chars one at a time
            if isinstance(self.pad, str):
                inp="".join((chr(x) for x in a_array))
            else:
                inp=bytes(a_array)

            inp+=self.pad[4:]
            
            rin_output=self.aes.encrypt(inp)
            out=rin_output[0]

            # Python 2 requires explicit conversion to int
            if isinstance(out, str):
                out=ord(out)

            return out >> 7
        
        addresses = [(address & mask[0]) | mask[1] for mask in self.masks]
        result=reduce(lambda x, y: x << 1 | y, (calc(a) for a in addresses), 0)
        return ".".join(["%s" % x for x in self.toarray(result ^ address)])
    

def main():
    key = b"boojahyoo3vaeToong0Eijee7Ahz3yee"
    crypto_pan = CryptoPan(key)
    anonymized_ip = crypto_pan.anonymize("192.0.2.1")
    print(anonymized_ip)
    
    # a resposta deve ser "206.2.124.120" conforme o teste da biblioteca original


if __name__ == "__main__":
    main()