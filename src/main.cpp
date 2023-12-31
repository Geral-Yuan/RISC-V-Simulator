#include "cpu.hpp"
//#include "test.hpp"

int main() {
#ifdef TEST_HPP
    for (int i = 0; i < 18; ++i) {
        printf("Testing case \"%s.c\"\n", file_name[i].c_str());
        std::ifstream fileIn("../testcases/" + file_name[i] + ".data");
        CPU cpu(fileIn);
        unsigned ans = cpu.pipelineRun();
        if (ans == testcase_ans[i])
            printf("%s\n", "AC");
        else
            printf("%s\ncorrect answer: %d\nmy answer: %d\n", "WA", testcase_ans[i], ans);
    }
#endif

#ifndef TEST_HPP
    CPU cpu;
    unsigned ans = cpu.pipelineRun();
    printf("%u\n", ans);
#endif

    return 0;
}