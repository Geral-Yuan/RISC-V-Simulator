#ifndef MEMORY_HPP
#define MEMORY_HPP

#define MEM_SIZE 1000000
typedef unsigned char Byte;

class Memory{
private:
    Byte mem[MEM_SIZE];
public:
    void write(unsigned pos, unsigned len, const unsigned &val);
    void read(unsigned pos, unsigned len, unsigned &val);
};

#endif // MEMORY_HPP
