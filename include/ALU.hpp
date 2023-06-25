#ifndef ALU_HPP
#define ALU_HPP

enum CalOp{
    AddOp,
    SubOp,
    LShiftOp,
    RshiftOp,
    AndOp,
    OrOp,
    XorOp
};

class ALU{
public:
    unsigned calculate(unsigned operand1, unsigned operand2, CalOp calop);
};

#endif // ALU_HPP
