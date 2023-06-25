#include "../include/memory.hpp"

void Memory::write(unsigned pos, unsigned len, const unsigned &val){
    unsigned i=0;
    while (i<len){
        mem[pos+i] = (Byte)(val >> (i << 3));
        ++i;
    }
}