#ifndef MEMORY_HPP
#define MEMORY_HPP

#define MEM_SIZE 1000000
typedef unsigned char Byte;

class Memory{
private:
    Byte mem[MEM_SIZE];
public:
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
