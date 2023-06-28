#ifndef PIPELINEBUFFER_HPP
#define PIPELINEBUFFER_HPP

#include "instructions.hpp"

struct IF_buffer {
    bool legal;
    unsigned insBits;
    unsigned insAddr;
    unsigned predictPC;

    IF_buffer() : legal(false), insBits(0), insAddr(0) {}
    void clear() {
        legal = false;
        insBits = insAddr = 0;
    }
};

struct ID_buffer {
    bool legal;
    unsigned rs1, rs2;
    unsigned regVal1, regVal2;
    unsigned imm;
    INSTRUCTION_TYPE insType;
    INSTRUCTION_CLASS insClass;
    unsigned insAddr;
    unsigned predictPC;
    unsigned regDes;

    ID_buffer() : legal(false), rs1(0), rs2(0), regVal1(0), regVal2(0), imm(0), insType(ILLEGAL), insAddr(0), regDes(0) {}
    void clear() {
        legal = false;
        regVal1 = regVal2 = insAddr = regDes = 0;
        insType = ILLEGAL;
    }
};

struct EX_buffer {
    bool legal;
    unsigned exRes;
    unsigned desAddr;
    INSTRUCTION_TYPE insType;
    INSTRUCTION_CLASS insClass;
    unsigned insAddr;
    unsigned regDes;

    EX_buffer() : legal(false), exRes(0), desAddr(0), insType(ILLEGAL), insAddr(0), regDes(0) {}
    void clear() {
        legal = false;
        exRes = desAddr = insAddr = regDes = 0;
        insType = ILLEGAL;
    }
};

struct MEM_buffer {
    bool legal;
    unsigned exRes;
    INSTRUCTION_TYPE insType;
    INSTRUCTION_CLASS insClass;
    unsigned insAddr;
    unsigned regDes;

    MEM_buffer() : legal(false), exRes(0), insType(ILLEGAL), insAddr(0), regDes(0) {}
    void clear() {
        legal = false;
        exRes = insAddr = regDes = 0;
        insType = ILLEGAL;
    }
};

struct WB_buffer {
    bool legal;
    unsigned exRes;
    INSTRUCTION_TYPE insType;
    INSTRUCTION_CLASS insClass;
    unsigned insAddr;
    unsigned regDes;

    WB_buffer() : legal(false), exRes(0), insType(ILLEGAL), insAddr(0), regDes(0) {}
    void clear() {
        legal = false;
        exRes = insAddr = regDes = 0;
        insType = ILLEGAL;
    }
};

#endif  // PIPELINEBUFFER_HPP
