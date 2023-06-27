#ifndef DECODER_HPP
#define DECODER_HPP

#include "instructions.hpp"

class Decoder {
   public:
    void decode(Instruction &ins) {
        if (ins.instructionBits == 0x0ff00513u) {
            ins.ins_type = END;
            return;
        }
        ins.opcode = ins.instructionBits & 0x7fu;
        ins.rd = (ins.instructionBits >> 7) & 0x1fu;
        ins.funct3 = (ins.instructionBits >> 12) & 0x07u;
        ins.funct7 = ins.instructionBits >> 25;
        ins.rs1 = (ins.instructionBits >> 15) & 0x1fu;
        ins.rs2 = (ins.instructionBits >> 20) & 0x1fu;
        // instruction can't match anyone is regarded as ILLEGAL for debugging
        if (ins.opcode == 0x37u || ins.opcode == 0x17u) {
            ins.ins_class = U_type;
            ins.imm = (ins.instructionBits >> 12) << 12;
            if (ins.opcode == 0x37u)
                ins.ins_type = LUI;
            else
                ins.ins_type = AUIPC;
        } else if (ins.opcode == 0x6fu) {
            ins.ins_class = J_type;
            ins.ins_type = JAL;
            ins.imm = ins.instructionBits & 0xff000u | (ins.instructionBits >> 9) & 0x800u | (ins.instructionBits >> 20) & 0x7feu | (ins.instructionBits >> 31 << 20);
        } else if (ins.opcode == 0x63u) {
            ins.ins_class = B_type;
            ins.imm = (ins.instructionBits >> 7) & 0x1eu | (ins.instructionBits >> 20) & 0x7e0u | (ins.instructionBits << 4) & 0x800u | (ins.instructionBits >> 31 << 12);
            switch (ins.funct3) {
                case 0x0u:
                    ins.ins_type = BEQ;
                    break;
                case 0x1u:
                    ins.ins_type = BNE;
                    break;
                case 0x4u:
                    ins.ins_type = BLT;
                    break;
                case 0x5u:
                    ins.ins_type = BGE;
                    break;
                case 0x6u:
                    ins.ins_type = BLTU;
                    break;
                case 0x7u:
                    ins.ins_type = BGEU;
                    break;
                default:
                    ins.ins_type = ILLEGAL;
            }
        } else if (ins.opcode == 0x67u || ins.opcode == 0x3u || ins.opcode == 0x13u) {
            ins.imm = ins.instructionBits >> 20;
            if (ins.opcode == 0x67u) {
                ins.ins_class = I_type1;
                ins.ins_type = JALR;
            } else if (ins.opcode == 0x3u) {
                ins.ins_class = I_type2;
                switch (ins.funct3) {
                    case 0x0u:
                        ins.ins_type = LB;
                        break;
                    case 0x1u:
                        ins.ins_type = LH;
                        break;
                    case 0x2u:
                        ins.ins_type = LW;
                        break;
                    case 0x4u:
                        ins.ins_type = LBU;
                        break;
                    case 0x5u:
                        ins.ins_type = LHU;
                        break;
                    default:
                        ins.ins_type = ILLEGAL;
                }
            } else {
                ins.ins_class = I_type3;
                switch (ins.funct3) {
                    case 0x0u:
                        ins.ins_type = ADDI;
                        break;
                    case 0x2u:
                        ins.ins_type = SLTI;
                        break;
                    case 0x3u:
                        ins.ins_type = SLTIU;
                        break;
                    case 0x4u:
                        ins.ins_type = XORI;
                        break;
                    case 0x6u:
                        ins.ins_type = ORI;
                        break;
                    case 0x7u:
                        ins.ins_type = ANDI;
                        break;
                    case 0x1u:
                        if (ins.funct7 == 0x0u)
                            ins.ins_type = SLLI;
                        else
                            ins.ins_type = ILLEGAL;
                        break;
                    case 0x5u:
                        if (ins.funct7 == 0x0u)
                            ins.ins_type = SRLI;
                        else if (ins.funct7 == 0x20u)
                            ins.ins_type = SRAI;
                        else
                            ins.ins_type = ILLEGAL;
                        break;
                    default:
                        ins.ins_type = ILLEGAL;
                }
            }
        } else if (ins.opcode == 0x23u) {
            ins.ins_class = S_type;
            ins.imm = (ins.instructionBits >> 7) & 0x1fu | (ins.instructionBits >> 25 << 5);
            switch (ins.funct3) {
                case 0x0u:
                    ins.ins_type = SB;
                    break;
                case 0x1u:
                    ins.ins_type = SH;
                    break;
                case 0x2u:
                    ins.ins_type = SW;
                    break;
                default:
                    ins.ins_type = ILLEGAL;
            }
        } else if (ins.opcode == 0x33u) {
            ins.ins_class = R_type;
            switch (ins.funct3) {
                case 0x0u:
                    if (ins.funct7 == 0x0u)
                        ins.ins_type = ADD;
                    else if (ins.funct7 == 0x20u)
                        ins.ins_type = SUB;
                    else
                        ins.ins_type = ILLEGAL;
                    break;
                case 0x1u:
                    if (ins.funct7 == 0x0u)
                        ins.ins_type = SLL;
                    else
                        ins.ins_type = ILLEGAL;
                    break;
                case 0x2u:
                    if (ins.funct7 == 0x0u)
                        ins.ins_type = SLT;
                    else
                        ins.ins_type = ILLEGAL;
                    break;
                case 0x3u:
                    if (ins.funct7 == 0x0u)
                        ins.ins_type = SLTU;
                    else
                        ins.ins_type = ILLEGAL;
                    break;
                case 0x4u:
                    if (ins.funct7 == 0x0u)
                        ins.ins_type = XOR;
                    else
                        ins.ins_type = ILLEGAL;
                    break;
                case 0x5u:
                    if (ins.funct7 == 0x0u)
                        ins.ins_type = SRL;
                    else if (ins.funct7 == 0x20u)
                        ins.ins_type = SRA;
                    else
                        ins.ins_type = ILLEGAL;
                    break;
                case 0x6u:
                    if (ins.funct7 == 0x0u)
                        ins.ins_type = OR;
                    else
                        ins.ins_type = ILLEGAL;
                    break;
                case 0x7u:
                    if (ins.funct7 == 0x0u)
                        ins.ins_type = AND;
                    else
                        ins.ins_type = ILLEGAL;
                    break;
                default:
                    ins.ins_type = ILLEGAL;
            }
        } else {
            ins.ins_type = ILLEGAL;
        }
    }
};

#endif  // DECODER_HPP
