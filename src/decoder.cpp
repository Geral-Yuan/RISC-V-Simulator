#include "../include/decoder.hpp"

void Decoder::decode(Instruction &ins){
    if (ins.instructionBits == (unsigned)0x0ff00513){
        ins.ins_type = END;
        return;
    }
    ins.opcode = ins.instructionBits & (unsigned)0x7f;
    ins.rd = (ins.instructionBits >> 7) & (unsigned)0x1f;
    ins.funct3 = (ins.instructionBits >> 12) & (unsigned)0x07;
    ins.rs1 = (ins.instructionBits >> 15) & (unsigned)0x1f;
    ins.rs2 = ins.shamt = (ins.instructionBits >> 20) & (unsigned)0x1f;
    // instruction can't match anyone is regarded as ILLEGAL for debugging
    if (ins.opcode == (unsigned)0x37 || ins.opcode == (unsigned)0x17){
        ins.ins_class = U_type;
        ins.imm = (ins.instructionBits >> 12) << 12;
        if (ins.opcode == (unsigned)0x37)
            ins.ins_type = LUI;
        else
            ins.ins_type = AUIPC;
    }else if (ins.opcode == (unsigned)0x6f){
        ins.ins_class = J_type;
        ins.ins_type = JAL;
        ins.imm = ins.instructionBits & (unsigned)0xff000 | (ins.instructionBits >> 9) & (unsigned)0x800 | (ins.instructionBits >> 20) & (unsigned)0x7fe | (ins.instructionBits >> 31 << 20);
    }else if (ins.opcode == (unsigned)0x63){
        ins.ins_class = B_type;
        ins.imm = (ins.instructionBits >> 7) & (unsigned)0x1e | (ins.instructionBits >> 20) & (unsigned)0x7e0 | (ins.instructionBits << 4) & (unsigned)0x800 | (ins.instructionBits >> 31 << 12);
        switch (ins.funct3){
        case (unsigned)0x0:
            ins.ins_type = BEQ;
            break;
        case (unsigned)0x1:
            ins.ins_type = BNE;
            break;
        case (unsigned)0x4:
            ins.ins_type = BLT;
            break;
        case (unsigned)0x5:
            ins.ins_type = BGE;
            break;
        case (unsigned)0x6:
            ins.ins_type = BLTU;
            break;
        case (unsigned)0x7:
            ins.ins_type = BGEU;
            break;
        default:
            ins.ins_type = ILLEGAL;
        }
    }else if (ins.opcode == (unsigned)0x67 || ins.opcode == (unsigned)0x3 || ins.opcode == (unsigned)0x13){
        ins.ins_class = I_type;
        ins.imm = ins.instructionBits >> 20;
        if (ins.opcode == (unsigned)0x67){
            ins.ins_type = JALR;
        }else if (ins.opcode == (unsigned)0x3){
            switch (ins.funct3){
            case (unsigned)0x0:
                ins.ins_type = LB;
                break;
            case (unsigned)0x1:
                ins.ins_type = LH;
                break;
            case (unsigned)0x2:
                ins.ins_type = LW;
                break;
            case (unsigned)0x4:
                ins.ins_type = LBU;
                break;
            case (unsigned)0x5:
                ins.ins_type = LHU;
                break;
            default:
                ins.ins_type = ILLEGAL;
            }
        }else{
            switch (ins.funct3){
            case (unsigned)0x0:
                ins.ins_type = ADDI;
                break;
            case (unsigned)0x2:
                ins.ins_type = SLTI;
                break;
            case (unsigned)0x3:
                ins.ins_type = SLTIU;
                break;
            case (unsigned)0x4:
                ins.ins_type = XORI;
                break;
            case (unsigned)0x6:
                ins.ins_type = ORI;
                break;
            case (unsigned)0x7:
                ins.ins_type = ANDI;
                break;
            case (unsigned)0x1:
                if (ins.funct7 == (unsigned)0x0)
                    ins.ins_type = SLLI;
                else
                    ins.ins_type = ILLEGAL;
                break;
            case (unsigned)0x5:
                if (ins.funct7 == (unsigned)0x0)
                    ins.ins_type = SRLI;
                else if (ins.funct7 == (unsigned)0x20)
                    ins.ins_type = SRAI;
                else
                    ins.ins_type = ILLEGAL;
                break;
            default:
                ins.ins_type = ILLEGAL;
            }
        }
    }else if (ins.opcode == (unsigned)0x23){
        ins.ins_class = S_type;
        ins.imm = (ins.instructionBits >> 7) & (unsigned)0x1f | (ins.instructionBits >> 25 << 5);
        switch (ins.funct3){
        case (unsigned)0x0:
            ins.ins_type = SB;
            break;
        case (unsigned)0x1:
            ins.ins_type = SH;
            break;
        case (unsigned)0x2:
            ins.ins_type = SW;
            break;
        default:
            ins.ins_type = ILLEGAL;
        }
    }else if (ins.opcode == (unsigned)0x33){
        switch (ins.funct3){
        case (unsigned)0x0:
            if (ins.funct7 == (unsigned)0x0)
                ins.ins_type = ADD;
            else if (ins.funct7 == (unsigned)0x20)
                ins.ins_type = SUB;
            else
                ins.ins_type = ILLEGAL;
            break;
        case (unsigned)0x1:
            if (ins.funct7 == (unsigned)0x0)
                ins.ins_type = SLL;
            else
                ins.ins_type = ILLEGAL;
            break;
        case (unsigned)0x2:
            if (ins.funct7 == (unsigned)0x0)
                ins.ins_type = SLT;
            else
                ins.ins_type = ILLEGAL;
            break;
        case (unsigned)0x3:
            if (ins.funct7 == (unsigned)0x0)
                ins.ins_type = SLTU;
            else
                ins.ins_type = ILLEGAL;
            break;
        case (unsigned)0x4:
            if (ins.funct7 == (unsigned)0x0)
                ins.ins_type = XOR;
            else
                ins.ins_type = ILLEGAL;
            break;
        case (unsigned)0x5:
            if (ins.funct7 == (unsigned)0x0)
                ins.ins_type = SRL;
            else if (ins.funct7 == (unsigned)0x20)
                ins.ins_type = SRA;
            else
                ins.ins_type = ILLEGAL;
            break;
        case (unsigned)0x6:
            if (ins.funct7 == (unsigned)0x0)
                ins.ins_type = OR;
            else
                ins.ins_type = ILLEGAL;
            break;
        case (unsigned)0x7:
            if (ins.funct7 == (unsigned)0x0)
                ins.ins_type = AND;
            else
                ins.ins_type = ILLEGAL;
            break;
        default:
            ins.ins_type = ILLEGAL;
        }
    }else{
        ins.ins_type = ILLEGAL;
    }
}
