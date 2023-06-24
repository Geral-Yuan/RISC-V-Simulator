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
        // ins.imm ...
    }else if (ins.opcode == (unsigned)0x63){
        ins.ins_class = B_type;
        // ...
    }
}
