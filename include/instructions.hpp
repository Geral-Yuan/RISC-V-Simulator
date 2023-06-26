#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

enum INSTRUCTION_TYPE {
    ILLEGAL, // illegal instruction 0
    LUI, AUIPC, // U-type 1-2
    JAL, // J-type 3
    BEQ, BNE, BLT, BGE, BLTU, BGEU, // B-type 4-9
    JALR, // I-type1 10
    LB, LH, LW, LBU, LHU, // I-type2 11-15
    ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI, // I-type3 16-24
    SB, SH, SW, // S-type 25-27
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND, // R-type 28-37
    END // end of main 0x0ff00513
};

enum INSTRUCTION_CLASS {
    U_type, J_type, B_type, I_type1, I_type2, I_type3, S_type, R_type
};

struct Instruction{
    unsigned instructionBits;
    unsigned opcode, imm, rs1, rs2, rd, funct3, funct7;
    unsigned result;
    INSTRUCTION_TYPE ins_type;
    INSTRUCTION_CLASS ins_class;
    Instruction() = default;
    explicit Instruction(const unsigned &insBits) { instructionBits = insBits; }
};

#endif // INSTRUCTIONS_HPP
