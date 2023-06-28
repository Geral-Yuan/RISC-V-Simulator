#ifndef PIPELINEBUFFER_HPP
#define PIPELINEBUFFER_HPP

#include "instructions.hpp"

struct IF_buffer {
    unsigned insBits;
    unsigned insAddr;
    unsigned predictPC;
    
    IF_buffer() : insBits(0), insAddr(0) {}
    void clear() {
        insBits = insAddr = 0;
    }
};

struct ID_buffer {
    unsigned regVal1, regVal2;
    unsigned imm;
    INSTRUCTION_TYPE insType;
    INSTRUCTION_CLASS insClass;
    unsigned insAddr;
    unsigned predictPC;
    unsigned regDes;

    ID_buffer() : regVal1(0), regVal2(0), imm(0), insType(ILLEGAL), insAddr(0), regDes(0) {}
    void clear() {
        regVal1 = regVal2 = insAddr = regDes = 0;
        insType = ILLEGAL;
    }
};

struct EX_buffer {
    unsigned exRes;
    unsigned desAddr;
    INSTRUCTION_TYPE insType;
    INSTRUCTION_CLASS insClass;
    unsigned insAddr;
    unsigned regDes;

    EX_buffer() : exRes(0), desAddr(0), insType(ILLEGAL), insAddr(0), regDes(0) {}
    void clear() {
        exRes = desAddr = insAddr = regDes = 0;
        insType = ILLEGAL;
    }
};

struct MEM_buffer {
    unsigned exRes;
    INSTRUCTION_TYPE insType;
    INSTRUCTION_CLASS insClass;
    unsigned insAddr;
    unsigned regDes;

    MEM_buffer() : exRes(0), insType(ILLEGAL), insAddr(0), regDes(0) {}
    void clear() {
        exRes = insAddr = regDes = 0;
        insType = ILLEGAL;
    }
};

struct WB_buffer {
    unsigned exRes;
    INSTRUCTION_TYPE insType;
    INSTRUCTION_CLASS insClass;
    unsigned insAddr;
    unsigned regDes;

    WB_buffer() : exRes(0), insType(ILLEGAL), insAddr(0), regDes(0) {}
    void clear() {
        exRes = insAddr = regDes = 0;
        insType = ILLEGAL;
    }
};

#endif  // PIPELINEBUFFER_HPP
