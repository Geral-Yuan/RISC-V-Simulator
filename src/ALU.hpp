#ifndef ALU_HPP
#define ALU_HPP

enum CalOp {
    AddOp,
    SubOp,
    LessOp,
    ULessOp,
    LShiftOp,
    RshiftOp,
    RshiftAop,
    AndOp,
    OrOp,
    XorOp
};

class ALU {
   public:
    unsigned calculate(unsigned operand1, unsigned operand2, CalOp calop) {
        switch (calop) {
            case AddOp:
                return operand1 + operand2;
            case SubOp:
                return operand1 - operand2;
            case LessOp:
                return (signed)operand1 < (signed)operand2;
            case ULessOp:
                return operand1 < operand2;
            case LShiftOp:
                return operand1 << (operand2 & 0x1fu);
            case RshiftOp:
                return operand1 >> (operand2 & 0x1fu);
            case RshiftAop:
                return ((long long)((signed)operand1)) >> (operand2 & 0x1fu);
            case AndOp:
                return operand1 & operand2;
            case OrOp:
                return operand1 | operand2;
            case XorOp:
                return operand1 ^ operand2;
        }
        return 0;
    }
};

#endif  // ALU_HPP
