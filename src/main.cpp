#include "cpu.hpp"
#include "test.hpp"

int main() {
#ifdef TEST_HPP
    for (int i = 0; i < 18; ++i) {
        printf("Testing case \"%s.c\"\n", file_name[i].c_str());
        std::ifstream fileIn("../testcases/" + file_name[i] + ".data");
        CPU cpu(fileIn);
        unsigned ans = cpu.debugRun();
        if (ans == testcase_ans[i])
            printf("%s\n\n", "AC");
        else
            printf("%s\n%d\n%d\n\n", "WA", testcase_ans[i], ans);
    }
#endif

#ifndef TEST_HPP
    CPU cpu;
    cpu.debugRun();
    unsigned ans = cpu.debugRun();
    printf("%u\n", ans);
#endif

    return 0;
}