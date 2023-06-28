#ifndef PIPELINESTAGE_HPP
#define PIPELINESTAGE_HPP

#include "memory.hpp"
#include "predictor.hpp"
#include "registers.hpp"
#include "decoder.hpp"
#include "ALU.hpp"
#include "pipelineBuffer.hpp"

void signedExtend_nBytes(unsigned numOfBytes, unsigned &val) {
    unsigned ans;
    if (numOfBytes == 1)
        ans = (char)val;
    else if (numOfBytes == 2)
        ans = (short)val;
    else
        ans = val;
    val = ans;
}

unsigned signedExtend_len(unsigned len, const unsigned &val) {
    if (val >> (len - 1)) return (-1) << len | val;
    return val;
}

class IF_stage {
   public:
    IF_buffer buffer;
    Memory &memory;
    Predictor &predictor;
    unsigned &pc, &nxt_pc;
    int countDown;

    IF_stage(Memory &mem, Predictor &pred, unsigned &_pc, unsigned &_nxt_pc, int cntD)
        : memory(mem), predictor(pred), pc(_pc), nxt_pc(_nxt_pc), countDown(cntD) {}

    void run() {
        if (countDown > 0) return;
        buffer.legal = true;
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
    int countDown;
    int &newCountDown;

    ID_stage(IF_buffer &_IF_ID, GPRs &_gprs, int cntD, int &ncntD)
        : IF_ID(_IF_ID), gprs(_gprs), countDown(cntD), newCountDown(ncntD) {}

    void run() {
        if (countDown > 0) return;
        if (!IF_ID.legal) return;
        Instruction ins(IF_ID.insBits);
        decoder.decode(ins);
        if (ins.ins_type == END) {
            newCountDown = 3;
            return;
        }
        buffer.legal = true;
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
    ALU alu;
    ID_buffer &ID_EX;
    Predictor &predictor;
    unsigned &nxt_pc, correct_nxt_pc;
    bool &clearWrongBranch;

    EX_stage(ID_buffer &_ID_EX, Predictor &pred, unsigned &_nxt_pc, bool &_clear)
        : ID_EX(_ID_EX), predictor(pred), nxt_pc(_nxt_pc), clearWrongBranch(_clear) {}

    void run() {
        if (!ID_EX.legal) return;
        buffer.legal = true;
        buffer.insAddr = ID_EX.insAddr;
        buffer.insType = ID_EX.insType;
        buffer.insClass = ID_EX.insClass;
        buffer.regDes = ID_EX.regDes;
        switch (ID_EX.insClass) {
            case U_type: {
                switch (ID_EX.insType) {
                    case LUI:
                        buffer.exRes = ID_EX.imm;
                    case AUIPC:
                        buffer.exRes += ID_EX.insAddr;
                }
                break;
            }
            case J_type: {
                buffer.exRes = ID_EX.insAddr + 4;
                correct_nxt_pc = ID_EX.insAddr + signedExtend_len(21, ID_EX.imm);
                if (nxt_pc != correct_nxt_pc) {
                    nxt_pc = correct_nxt_pc;
                    clearWrongBranch = true;
                }
                break;
            }
            case I_type1: {
                buffer.exRes = ID_EX.insAddr + 4;
                correct_nxt_pc = (ID_EX.regVal1 + signedExtend_len(12, ID_EX.imm)) & (-2);
                if (nxt_pc != correct_nxt_pc) {
                    nxt_pc = correct_nxt_pc;
                    clearWrongBranch = true;
                }
                break;
            }
            case B_type: {
                bool jump = false;
                switch (ID_EX.insType) {
                    case BEQ:
                        jump = (ID_EX.regVal1 == ID_EX.regVal2);
                        break;
                    case BNE:
                        jump = (ID_EX.regVal1 != ID_EX.regVal2);
                        break;
                    case BLT:
                        jump = ((signed)ID_EX.regVal1 < (signed)ID_EX.regVal2);
                        break;
                    case BGE:
                        jump = ((signed)ID_EX.regVal1 >= (signed)ID_EX.regVal2);
                        break;
                    case BLTU:
                        jump = (ID_EX.regVal1 < ID_EX.regVal2);
                        break;
                    case BGEU:
                        jump = (ID_EX.regVal1 >= ID_EX.regVal2);
                        break;
                }
                if (jump)
                    correct_nxt_pc = ID_EX.insAddr + signedExtend_len(13, ID_EX.imm);
                else
                    correct_nxt_pc = ID_EX.insAddr + 4;
                if (!predictor.judgePred(ID_EX.insAddr, ID_EX.insType, correct_nxt_pc, ID_EX.predictPC)) {
                    nxt_pc = correct_nxt_pc;
                    clearWrongBranch = true;
                }
                break;
            }
            case I_type3: {
                CalOp opi;
                switch (ID_EX.insType) {
                    case ADDI:
                        opi = AddOp;
                        break;
                    case SLLI:
                        opi = LShiftOp;
                        break;
                    case SRLI:
                        opi = RshiftOp;
                        break;
                    case SRAI:
                        opi = RshiftAop;
                        break;
                    case SLTI:
                        opi = LessOp;
                        break;
                    case SLTIU:
                        opi = ULessOp;
                        break;
                    case XORI:
                        opi = XorOp;
                        break;
                    case ORI:
                        opi = OrOp;
                        break;
                    case ANDI:
                        opi = AndOp;
                        break;
                }
                buffer.exRes = alu.calculate(ID_EX.regVal1, ID_EX.imm, opi);
                break;
            }
            case R_type: {
                CalOp op;
                switch (ID_EX.insType) {
                    case ADD:
                        op = AddOp;
                        break;
                    case SUB:
                        op = SubOp;
                        break;
                    case SLL:
                        op = LShiftOp;
                        break;
                    case SRL:
                        op = RshiftOp;
                        break;
                    case SRA:
                        op = RshiftAop;
                        break;
                    case SLT:
                        op = LessOp;
                        break;
                    case SLTU:
                        op = ULessOp;
                        break;
                    case XOR:
                        op = XorOp;
                        break;
                    case OR:
                        op = OrOp;
                        break;
                    case AND:
                        op = AndOp;
                        break;
                }
                buffer.exRes = alu.calculate(ID_EX.regVal1, ID_EX.regVal2, op);
                break;
            }
            case S_type:
                buffer.exRes = ID_EX.regVal2;
            case I_type2:
                buffer.desAddr = ID_EX.regVal1 + signedExtend_len(12, ID_EX.imm);
                break;
        }
    }
};

class MEM_stage {
   public:
    MEM_buffer buffer;
    EX_buffer &EX_MEM;
    Memory &memory;
    unsigned &stallCnt;

    MEM_stage(EX_buffer &_EX_MEM, Memory &mem, unsigned &_stallCnt)
        : EX_MEM(_EX_MEM), memory(mem), stallCnt(_stallCnt) {}

    void run() {
        if (!EX_MEM.legal) return;
        buffer.legal = true;
        buffer.insAddr = EX_MEM.insAddr;
        buffer.insType = EX_MEM.insType;
        buffer.insClass = EX_MEM.insClass;
        buffer.regDes = EX_MEM.regDes;
        buffer.exRes = EX_MEM.exRes;
        switch (EX_MEM.insClass) {
            case S_type: {
                unsigned len_store;
                switch (EX_MEM.insType) {
                    case SB:
                        len_store = 1;
                        break;
                    case SH:
                        len_store = 2;
                        break;
                    case SW:
                        len_store = 4;
                        break;
                }
                memory.write(EX_MEM.desAddr, len_store, EX_MEM.exRes);
                break;
            }
            case I_type2: {
                unsigned len_load;
                bool unsigned_flag = false;
                switch (EX_MEM.insType) {
                    case LB:
                        len_load = 1;
                        break;
                    case LH:
                        len_load = 2;
                        break;
                    case LW:
                        len_load = 4;
                        break;
                    case LBU:
                        len_load = 1;
                        unsigned_flag = true;
                        break;
                    case LHU:
                        len_load = 2;
                        unsigned_flag = true;
                        break;
                }
                memory.read(EX_MEM.desAddr, len_load, buffer.exRes);
                break;
            }
            default:
                stallCnt = 2;
        }
    }
};

class WB_stage {
   public:
    WB_buffer buffer;
    MEM_buffer &MEM_WB;
    GPRs &gprs;

    WB_stage(MEM_buffer &_MEM_WB, GPRs &_gprs)
        : MEM_WB(_MEM_WB), gprs(_gprs) {}

    void run() {
        if (!MEM_WB.legal) return;
        buffer.legal = true;
        buffer.insAddr = MEM_WB.insAddr;
        buffer.insType = MEM_WB.insType;
        buffer.insClass = MEM_WB.insClass;
        buffer.regDes = MEM_WB.regDes;
        buffer.exRes = MEM_WB.exRes;
        switch (MEM_WB.insClass) {
            case U_type:
            case J_type:
            case I_type1:
            case I_type2:
            case I_type3:
            case R_type:
                gprs.writeVal(MEM_WB.regDes,MEM_WB.exRes);
                break;
        }
    }
};

#endif  // PIPELINESTAGE_HPP
