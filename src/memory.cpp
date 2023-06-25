#include "../include/memory.hpp"

void Memory::write(unsigned pos, unsigned len, const unsigned &val){
    unsigned i=0;
    while (i<len){
        mem[pos+i] = (Byte)(val >> (i << 3u));
        ++i;
    }
}

void Memory::read(unsigned pos, unsigned len, unsigned &val){
    val = 0;
    int i = len;
    while (i--) val = val << 8u | mem[pos+i];
}
