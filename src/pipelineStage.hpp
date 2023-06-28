#ifndef PIPELINESTAGE_HPP
#define PIPELINESTAGE_HPP

#include "memory.hpp"
#include "predictor.hpp"
#include "registers.hpp"
#include "decoder.hpp"
#include "ALU.hpp"
#include "pipelineBuffer.hpp"

class IF_stage {
   public:
    IF_buffer buffer;
    Memory &memory;
    Predictor &predictor;
    unsigned &pc, &nxt_pc;
    bool &running;
    int countDown;

    IF_stage(Memory &mem, Predictor &pred, unsigned &_pc, unsigned &_nxt_pc, bool &_running, int cntD)
        : memory(mem), predictor(pred), pc(_pc), nxt_pc(_nxt_pc), running(_running), countDown(cntD) {}

    void run() {
        if (countDown > 0) return;
        buffer.insAddr = pc;
        memory.read(pc, 4, buffer.insBits);
        if (!nxt_pc) nxt_pc = buffer.predictPC = predictor.nxtPC(pc, buffer.insBits);
    }
};

class ID_stage {
   public:
    ID_buffer buffer;
    Decoder decoder;
    IF_buffer &IF_ID;
    GPRs &gprs;
    unsigned &pc, &nxt_pc;
    bool &running;
    int countDown;
    int &newCountDown;

    ID_stage(IF_buffer &_IF_ID, GPRs &_gprs, unsigned &_pc, unsigned &_nxt_pc, bool &_running, int cntD, int &ncntD)
        : IF_ID(_IF_ID), gprs(_gprs), pc(_pc), nxt_pc(_nxt_pc), running(_running), countDown(cntD), newCountDown(ncntD) {}

    void run() {
        if (countDown > 0) return;
        Instruction ins(IF_ID.insBits);
        decoder.decode(ins);
        if (ins.ins_type == END) {
            newCountDown = 2;
            return;
        }
        buffer.insAddr = IF_ID.insAddr;
        buffer.predictPC = IF_ID.predictPC;
        buffer.insType = ins.ins_type;
        buffer.insClass = ins.ins_class;
        switch (ins.ins_class) {
            case U_type:
            case J_type:
                buffer.imm = ins.imm;
                break;
            case B_type:
            case S_type:
                buffer.imm = ins.imm;
                buffer.regVal1 = gprs.getVal(ins.rs1);
                buffer.regVal2 = gprs.getVal(ins.rs2);
                break;
            case I_type1:
            case I_type2:
            case I_type3:
                buffer.imm = ins.imm;
                buffer.regVal1 = gprs.getVal(ins.rs1);
                buffer.regDes = ins.rd;
                break;
            case R_type:
                buffer.regVal1 = gprs.getVal(ins.rs1);
                buffer.regVal2 = gprs.getVal(ins.rs2);
                buffer.regDes = ins.rd;
                break;
        }
    }
};

class EX_stage {
   public:
    EX_buffer buffer;
    ID_buffer &ID_EX;
    Predictor &predictor;
    unsigned &pc, &nxt_pc;
    bool &running;
    int countDown;

    EX_stage(ID_buffer &_ID_EX, Predictor &pred, unsigned &_pc, unsigned &_nxt_pc, bool &_running, int cntD)
        : ID_EX(_ID_EX), predictor(pred), pc(_pc), nxt_pc(_nxt_pc), running(_running), countDown(cntD) {}

    void run() {
        if (countDown > 0) return;
    }
};

class MEM_stage {
   public:
    MEM_buffer buffer;
    EX_buffer &EX_MEM;
    Memory &memory;
    unsigned &pc, &nxt_pc;
    bool &running;
    int countDown;

    MEM_stage(EX_buffer &_EX_MEM, Memory &mem, unsigned &_pc, unsigned &_nxt_pc, bool &_running, int cntD)
        : EX_MEM(_EX_MEM), memory(mem), pc(_pc), nxt_pc(_nxt_pc), running(_running), countDown(cntD) {}

    void run() {
        if (countDown == 1) return;
    }
};

class WB_stage {
   public:
    WB_buffer buffer;
    MEM_buffer &MEM_WB;
    GPRs &gprs;
    unsigned &pc, &nxt_pc;
    bool &running;
    int countDown;

    WB_stage(MEM_buffer &_MEM_WB, GPRs &_gprs, unsigned &_pc, unsigned &_nxt_pc, bool &_running, int cntD)
        : MEM_WB(_MEM_WB), gprs(_gprs), pc(_pc), nxt_pc(_nxt_pc), running(_running), countDown(cntD) {}

    void run() {
    }
};

#endif  // PIPELINESTAGE_HPP
