#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include <cstdio>
#include <cstring>
#include "instructions.hpp"

// naive version
// class Predictor {
//    private:
//     int totalCnt, correctCnt;

//    public:
//     Predictor() : totalCnt(0), correctCnt(0) {}
//     ~Predictor() { printf("Branch prediction correct rate: %lf%%\n\n", double(correctCnt) / totalCnt * 100); }
//     unsigned predictNextPC(unsigned pc, unsigned insBits) { return pc + 4; }
//     bool judgePred(unsigned pc, INSTRUCTION_TYPE ins, unsigned correctAns, unsigned predAns) {
//         ++totalCnt;
//         if (predAns == correctAns) ++correctCnt;
//         return predAns == correctAns;
//     }
// };

// 2-bit saturate counter version
class Predictor {
   private:
    int totalCnt, correctCnt;
    size_t size;
    unsigned char *twoBitCounter;
    unsigned *targetAddress;

   public:
    Predictor(int bitsCnt) : totalCnt(0), correctCnt(0), size(2 << bitsCnt) {
        twoBitCounter = new unsigned char[size];
        targetAddress = new unsigned[size];
        memset(twoBitCounter, 0, size * sizeof(unsigned char));
        memset(targetAddress, 0, size * sizeof(unsigned));
    }
    ~Predictor() {
        printf("Branch prediction correct rate: %lf%%\n\n", double(correctCnt) / totalCnt * 100);
        delete[] twoBitCounter;
        delete[] targetAddress;
    }
    unsigned predictNextPC(unsigned pc, unsigned insBits) {
        unsigned opcode = insBits & 0x7fu;
        unsigned hash = (pc >> 2) & (size - 1);
        if (opcode == 0x6fu || opcode == 0x67u) return targetAddress[hash] ? targetAddress[hash] : pc + 4;
        if (opcode == 0x63u) return targetAddress[hash] && (twoBitCounter[hash] & 2) ? targetAddress[hash] : pc + 4;
        return pc + 4;
    }
    void storeTargetAddress(unsigned pc, unsigned _targetAddress) { targetAddress[(pc >> 2) & (size - 1)] = _targetAddress; }
    bool judgePred(unsigned pc, INSTRUCTION_TYPE ins, unsigned correctAns, unsigned predAns) {
        unsigned hash = (pc >> 2) & (size - 1);
        if (correctAns == pc + 4 && twoBitCounter[hash]) --twoBitCounter[hash];
        if (correctAns == targetAddress[hash] && twoBitCounter[hash] != 3) ++twoBitCounter[hash];
        ++totalCnt;
        if (predAns == correctAns) ++correctCnt;
        return predAns == correctAns;
    }
};

#endif  // PREDICTOR_HPP
