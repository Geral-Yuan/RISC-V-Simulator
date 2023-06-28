#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include <cstdio>
#include "instructions.hpp"

// naive version
class Predictor {
   private:
    int totalCnt, correctCnt;

   public:
    Predictor() : totalCnt(0), correctCnt(0) {}
    // ~Predictor() { printf("\nBranch prediction correct rate: %lf\n", double(correctCnt) / totalCnt); }
    unsigned nxtPC(unsigned pc, unsigned insBits) { return pc + 4; }
    bool judgePred(unsigned pc, INSTRUCTION_TYPE ins, unsigned correctAns, unsigned predAns) {
        ++totalCnt;
        if (predAns == correctAns) ++correctAns;
        return predAns == correctAns;
    }
};

#endif  // PREDICTOR_HPP
