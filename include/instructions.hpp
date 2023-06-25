#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

enum INSTRUCTION_TYPE {
    ILLEGAL, // illegal instruction 0
    LUI, AUIPC, // U-type 1-2
    JAL, // J-type 3
    BEQ, BNE, BLT, BGE, BLTU, BGEU, // B-type 4-9
    JALR, LB, LH, LW, LBU, LHU, ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI, // I-type 10-24
    SB, SH, SW, // S-type 25-27
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND, // R-type 28-37
    END // end of main 0x0ff00513
};

enum INSTRUCTION_CLASS {
    U_type, J_type, B_type, I_type, S_type, R_type
};

struct Instruction{
    unsigned instructionBits;
    unsigned opcode, imm, shamt, rs1, rs2, rd, funct3, funct7;
    INSTRUCTION_TYPE ins_type;
    INSTRUCTION_CLASS ins_class;
    Instruction() = default;
    explicit Instruction(const unsigned &insBits) { instructionBits = insBits; }
};

#endif // INSTRUCTIONS_HPP
