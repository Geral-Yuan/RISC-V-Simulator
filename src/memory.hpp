#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <iostream>
#include <fstream>
#include <cstring>

const size_t MEM_SIZE = 2 << 18;
typedef unsigned char Byte;

class Memory{
private:
    Byte mem[MEM_SIZE];

    unsigned Hex2Dec(const std::string &HexStr){
        unsigned DecInt = 0;
        unsigned len = HexStr.size();
        for (int i = 0; i < len; ++i){
            DecInt <<= 4;
            if (HexStr[i] <= '9')
                DecInt += HexStr[i] - '0';
            else
                DecInt += 10 + HexStr[i] - 'A';
        }
        return DecInt;
    }

public:
    explicit Memory(std::istream &is){
        memset(mem,0,sizeof(mem));
        std::string strLine;
        unsigned pos = 0;
        while (getline(is,strLine)){
            if (strLine[0] == '@')
                pos = Hex2Dec(strLine.substr(1, 8));
            else
                for (int i = 0; i < strLine.size(); i += 3)
                    write(pos++, 1, Hex2Dec(strLine.substr(i, 2)));
        }
    }

    void write(unsigned pos, unsigned len, const unsigned &val){
        unsigned i=0;
        while (i<len){
            mem[pos+i] = (Byte)(val >> (i << 3u));
            ++i;
        }
    }

    void read(unsigned pos, unsigned len, unsigned &val){
        val = 0;
        int i = len;
        while (i--) val = val << 8u | mem[pos+i];
    }
};

#endif // MEMORY_HPP
