#ifndef REGISTERS_HPP
#define REGISTERS_HPP

// GPRs for General Purpose Registers
class GPRs{
private:
    unsigned reg[32];
public:
    void writeVal(unsigned idx, const unsigned &val){ reg[idx] = val; }
    unsigned getVal(unsigned idx){ return reg[idx]; }
};

#endif // REGISTERS_HPP
