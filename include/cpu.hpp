#ifndef CPU_HPP
#define CPU_HPP

#include "memory.hpp"
#include "registers.hpp"
#include "decoder.hpp"
#include "ALU.hpp"
#include <string>

class CPU{
private:
    Memory memory;
    GPRs gprs;
    Decoder decoder;
    ALU alu;
    unsigned pc;
    
    void signedExtend_nBytes(unsigned numOfBytes, unsigned &val);
    unsigned signedExtend_len(unsigned len, const unsigned &val);
    unsigned Hex2Dec(const std::string &HexStr);

public:
    CPU(){ pc = 0u; }
    void getInput();
    void debugRun();
};

#endif // CPU_HPP
