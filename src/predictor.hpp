#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP
#define HYBRID_PREDICTOR

#include <cstdio>
#include <cstring>
#include "instructions.hpp"

#ifdef NAIVE_PREDICTOR
// naive predictor (always not jump)
class Predictor {
   private:
    int totalCnt, correctCnt;

   public:
    Predictor() : totalCnt(0), correctCnt(0) {}
    // ~Predictor() { printf("Branch prediction correct rate: %lf%%\n\n", double(correctCnt) / totalCnt * 100); }
    unsigned predictNextPC(unsigned pc, unsigned insBits) { return pc + 4; }
    bool judgePred(unsigned pc, INSTRUCTION_TYPE ins, unsigned correctAns, unsigned predAns) {
        ++totalCnt;
        if (predAns == correctAns) ++correctCnt;
        return predAns == correctAns;
    }
};
#endif

#ifdef TWO_BIT_SATURATE_COUNTER_PREDICTOR
// 2-bit saturate counter predictor
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
        // printf("Branch prediction correct rate: %lf%%\n\n", double(correctCnt) / totalCnt * 100);
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
#endif

#ifdef LOCAL_HISTORY_PREDICTOR
// local history predictor
class Predictor {
   private:
    int totalCnt, correctCnt;
    size_t size;
    unsigned char *BHT;
    unsigned char *PHTs[16];
    unsigned *targetAddress;

   public:
    Predictor(int bitsCnt) : totalCnt(0), correctCnt(0), size(2 << bitsCnt) {
        BHT = new unsigned char[size];
        for (int i = 0; i < 16; ++i) {
            PHTs[i] = new unsigned char[size];
            memset(PHTs[i], 0, size * sizeof(unsigned char));
        }
        targetAddress = new unsigned[size];
        memset(BHT, 0, size * sizeof(unsigned char));
        memset(targetAddress, 0, size * sizeof(unsigned));
    }
    ~Predictor() {
        // printf("Branch prediction correct rate: %lf%%\n\n", double(correctCnt) / totalCnt * 100);
        delete[] BHT;
        for (int i = 0; i < 16; ++i) delete[] PHTs[i];
        delete[] targetAddress;
    }
    unsigned predictNextPC(unsigned pc, unsigned insBits) {
        unsigned opcode = insBits & 0x7fu;
        unsigned hash = (pc >> 2) & (size - 1);
        if (opcode == 0x6fu || opcode == 0x67u) return targetAddress[hash] ? targetAddress[hash] : pc + 4;
        if (opcode == 0x63u) return targetAddress[hash] && (PHTs[BHT[hash] & 0xfu][hash] & 2) ? targetAddress[hash] : pc + 4;
        return pc + 4;
    }
    void storeTargetAddress(unsigned pc, unsigned _targetAddress) { targetAddress[(pc >> 2) & (size - 1)] = _targetAddress; }
    bool judgePred(unsigned pc, INSTRUCTION_TYPE ins, unsigned correctAns, unsigned predAns) {
        unsigned hash = (pc >> 2) & (size - 1);
        if (correctAns == pc + 4) {
            if (PHTs[BHT[hash] & 0xfu][hash]) --PHTs[BHT[hash] & 0xfu][hash];
            BHT[hash] <<= 1;
        }
        if (correctAns == targetAddress[hash]) {
            if (PHTs[BHT[hash] & 0xfu][hash] != 3) ++PHTs[BHT[hash] & 0xfu][hash];
            BHT[hash] = BHT[hash] << 1 | 1;
        }
        ++totalCnt;
        if (predAns == correctAns) ++correctCnt;
        return predAns == correctAns;
    }
};
#endif

#ifdef GLOBAL_HISTORY_PREDICTOR
// global history predictor
class Predictor {
   private:
    int totalCnt, correctCnt;
    size_t size;
    unsigned char GHR;
    unsigned char *PHTs[16];
    unsigned *targetAddress;

   public:
    Predictor(int bitsCnt) : totalCnt(0), correctCnt(0), size(2 << bitsCnt), GHR(0) {
        for (int i = 0; i < 16; ++i) {
            PHTs[i] = new unsigned char[size];
            memset(PHTs[i], 0, size * sizeof(unsigned char));
        }
        targetAddress = new unsigned[size];
        memset(targetAddress, 0, size * sizeof(unsigned));
    }
    ~Predictor() {
        // printf("Branch prediction correct rate: %lf%%\n\n", double(correctCnt) / totalCnt * 100);
        for (int i = 0; i < 16; ++i) delete[] PHTs[i];
        delete[] targetAddress;
    }
    unsigned predictNextPC(unsigned pc, unsigned insBits) {
        unsigned opcode = insBits & 0x7fu;
        unsigned hash = (pc >> 2) & (size - 1);
        if (opcode == 0x6fu || opcode == 0x67u) return targetAddress[hash] ? targetAddress[hash] : pc + 4;
        if (opcode == 0x63u) return targetAddress[hash] && (PHTs[GHR & 0xfu][hash] & 2) ? targetAddress[hash] : pc + 4;
        return pc + 4;
    }
    void storeTargetAddress(unsigned pc, unsigned _targetAddress) { targetAddress[(pc >> 2) & (size - 1)] = _targetAddress; }
    bool judgePred(unsigned pc, INSTRUCTION_TYPE ins, unsigned correctAns, unsigned predAns) {
        unsigned hash = (pc >> 2) & (size - 1);
        if (correctAns == pc + 4) {
            if (PHTs[GHR & 0xfu][hash]) --PHTs[GHR & 0xfu][hash];
            GHR <<= 1;
        }
        if (correctAns == targetAddress[hash]) {
            if (PHTs[GHR & 0xfu][hash] != 3) ++PHTs[GHR & 0xfu][hash];
            GHR = GHR << 1 | 1;
        }
        ++totalCnt;
        if (predAns == correctAns) ++correctCnt;
        return predAns == correctAns;
    }
};
#endif

#ifdef HYBRID_PREDICTOR
// hybrid predictor
class Predictor {
   private:
    int totalCnt, correctCnt;
    size_t size;
    unsigned *callCnt;
    unsigned char *twoBitCounter;
    unsigned char *BHT;
    unsigned char *PHTs[16];
    unsigned *targetAddress;

   public:
    Predictor(int bitsCnt) : totalCnt(0), correctCnt(0), size(2 << bitsCnt) {
        callCnt = new unsigned[size];
        twoBitCounter = new unsigned char[size];
        BHT = new unsigned char[size];
        for (int i = 0; i < 16; ++i) {
            PHTs[i] = new unsigned char[size];
            memset(PHTs[i], 0, size * sizeof(unsigned char));
        }
        targetAddress = new unsigned[size];
        memset(callCnt, 0, size * sizeof(unsigned));
        memset(twoBitCounter, 0, size * sizeof(unsigned char));
        memset(BHT, 0, size * sizeof(unsigned char));
        memset(targetAddress, 0, size * sizeof(unsigned));
    }
    ~Predictor() {
        // printf("Branch prediction correct rate: %lf%%\n\n", double(correctCnt) / totalCnt * 100);
        delete[] callCnt;
        delete[] twoBitCounter;
        delete[] BHT;
        for (int i = 0; i < 16; ++i) delete[] PHTs[i];
        delete[] targetAddress;
    }
    unsigned predictNextPC(unsigned pc, unsigned insBits) {
        unsigned opcode = insBits & 0x7fu;
        unsigned hash = (pc >> 2) & (size - 1);
        if (opcode == 0x6fu || opcode == 0x67u) return targetAddress[hash] ? targetAddress[hash] : pc + 4;
        if (opcode == 0x63u)
            if (callCnt[hash] > 32)
                return targetAddress[hash] && (PHTs[BHT[hash] & 0xfu][hash] & 2) ? targetAddress[hash] : pc + 4;
            else
                return targetAddress[hash] && (twoBitCounter[hash] & 2) ? targetAddress[hash] : pc + 4;
        return pc + 4;
    }
    void storeTargetAddress(unsigned pc, unsigned _targetAddress) { targetAddress[(pc >> 2) & (size - 1)] = _targetAddress; }
    bool judgePred(unsigned pc, INSTRUCTION_TYPE ins, unsigned correctAns, unsigned predAns) {
        unsigned hash = (pc >> 2) & (size - 1);
        ++callCnt[hash];
        if (correctAns == pc + 4) {
            if (PHTs[BHT[hash] & 0xfu][hash]) --PHTs[BHT[hash] & 0xfu][hash];
            if (callCnt[hash] <= 20 && twoBitCounter[hash]) --twoBitCounter[hash];
            BHT[hash] <<= 1;
        }
        if (correctAns == targetAddress[hash]) {
            if (PHTs[BHT[hash] & 0xfu][hash] != 3) ++PHTs[BHT[hash] & 0xfu][hash];
            if (callCnt[hash] <= 32 && twoBitCounter[hash] != 3) ++twoBitCounter[hash];
            BHT[hash] = BHT[hash] << 1 | 1;
        }
        ++totalCnt;
        if (predAns == correctAns) ++correctCnt;
        return predAns == correctAns;
    }
};
#endif

#endif  // PREDICTOR_HPP
