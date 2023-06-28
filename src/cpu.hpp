#ifndef CPU_HPP
#define CPU_HPP

#include "pipelineStage.hpp"

class CPU {
   private:
    Memory memory;
    GPRs gprs;
    Predictor predictor;
    unsigned pc, nxt_pc;
    int countDown, newCountDown;
    bool IF_run, ID_run, EX_run, MEM_run, WB_run;

    IF_stage IF;
    ID_stage ID;
    EX_stage EX;
    MEM_stage MEM;
    WB_stage WB;

    IF_buffer IF_ID;
    ID_buffer ID_EX;
    EX_buffer EX_MEM;
    MEM_buffer MEM_WB;
    WB_buffer postWB;

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

   public:
    explicit CPU(std::istream &is = std::cin)
        : memory(is), pc(0), nxt_pc(0), countDown(-1), newCountDown(-1), IF_run(true), ID_run(true), EX_run(true), MEM_run(true), WB_run(true), IF(memory, predictor, pc, nxt_pc, IF_run, countDown), ID(IF_ID, gprs, pc, nxt_pc, ID_run, countDown, newCountDown), EX(ID_EX, predictor, pc, nxt_pc, EX_run, countDown), MEM(EX_MEM, memory, pc, nxt_pc, MEM_run, countDown), WB(MEM_WB, gprs, pc, nxt_pc, WB_run, countDown) {}

    unsigned pipelineRun() {
        while (true) {
            // following five functions can run in any order
            IF.run();
            ID.run();
            EX.run();
            MEM.run();
            WB.run();

            countDown = newCountDown;
            if (countDown > 0) --countDown;
            if (countDown == 0) break;

            if (IF_run)
                IF_ID = IF.buffer;
            if (ID_run)
                ID_EX = ID.buffer;
            if (EX_run)
                EX_MEM = EX.buffer;
            if (MEM_run)
                MEM_WB = MEM.buffer;
            if (WB_run)
                postWB = WB.buffer;
        }
        return gprs.getVal(10) & 255u;
    }

    unsigned naiveRun() {
        Decoder decoder;
        ALU alu;
        Instruction ins;
        while (1) {
            memory.read(pc, 4, ins.instructionBits);
            decoder.decode(ins);
            if (ins.ins_type == END) break;
            switch (ins.ins_class) {
                case U_type: {
                    switch (ins.ins_type) {
                        case LUI:
                            gprs.writeVal(ins.rd, ins.imm);
                            break;
                        case AUIPC:
                            gprs.writeVal(ins.rd, pc + ins.imm);
                            break;
                    }
                    pc += 4;
                    break;
                }
                case J_type: {
                    gprs.writeVal(ins.rd, pc + 4);
                    pc += signedExtend_len(21, ins.imm);
                    break;
                }
                case B_type: {
                    bool jump = false;
                    switch (ins.ins_type) {
                        case BEQ:
                            jump = (gprs.getVal(ins.rs1) == gprs.getVal(ins.rs2));
                            break;
                        case BNE:
                            jump = (gprs.getVal(ins.rs1) != gprs.getVal(ins.rs2));
                            break;
                        case BLT:
                            jump = ((signed)gprs.getVal(ins.rs1) < (signed)gprs.getVal(ins.rs2));
                            break;
                        case BGE:
                            jump = ((signed)gprs.getVal(ins.rs1) >= (signed)gprs.getVal(ins.rs2));
                            break;
                        case BLTU:
                            jump = (gprs.getVal(ins.rs1) < gprs.getVal(ins.rs2));
                            break;
                        case BGEU:
                            jump = (gprs.getVal(ins.rs1) >= gprs.getVal(ins.rs2));
                            break;
                    }
                    if (jump)
                        pc += signedExtend_len(13, ins.imm);
                    else
                        pc += 4;
                    break;
                }
                case I_type1: {
                    unsigned nxt_pc = pc + 4;
                    pc = (gprs.getVal(ins.rs1) + signedExtend_len(12, ins.imm)) & (-2);
                    gprs.writeVal(ins.rd, nxt_pc);
                    break;
                }
                case I_type2: {
                    unsigned val_to_write;
                    unsigned address_to_load = gprs.getVal(ins.rs1) + signedExtend_len(12, ins.imm);
                    unsigned len_load;
                    bool unsigned_flag = false;
                    switch (ins.ins_type) {
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
                    memory.read(address_to_load, len_load, val_to_write);
                    if (!unsigned_flag) signedExtend_nBytes(len_load, val_to_write);
                    gprs.writeVal(ins.rd, val_to_write);
                    pc += 4;
                    break;
                }
                case I_type3: {
                    CalOp opi;
                    switch (ins.ins_type) {
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
                    gprs.writeVal(ins.rd, alu.calculate(gprs.getVal(ins.rs1), signedExtend_len(12, ins.imm), opi));
                    pc += 4;
                    break;
                }
                case R_type: {
                    CalOp op;
                    switch (ins.ins_type) {
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
                    gprs.writeVal(ins.rd, alu.calculate(gprs.getVal(ins.rs1), gprs.getVal(ins.rs2), op));
                    pc += 4;
                    break;
                }
                case S_type: {
                    unsigned address_to_store = gprs.getVal(ins.rs1) + signedExtend_len(12, ins.imm);
                    unsigned len_store;
                    switch (ins.ins_type) {
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
                    memory.write(address_to_store, len_store, gprs.getVal(ins.rs2));
                    pc += 4;
                    break;
                }
            }
            gprs.writeVal(0, 0);
        }
        return gprs.getVal(10) & 255u;
    }
};

#endif  // CPU_HPP
