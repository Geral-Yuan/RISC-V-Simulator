# RISC-V Simulator

## Introduction

This program is a RISC-V simulator written by C++, which implements a classic five stage pipeline and simulates simplified RV32I instructions.

### About RISC-V

RISC-V (pronounced "risk-five",) is an open standard instruction set architecture (ISA) based on established reduced instruction set computer (RISC) principles. Unlike most other ISA designs, RISC-V is provided under royalty-free open-source licenses. A number of companies are offering or have announced RISC-V hardware, open source operating systems with RISC-V support are available, and the instruction set is supported in several popular software toolchains.[^1]

Click [here](https://en.wikipedia.org/wiki/RISC-V) for more.

### Supported Instructions

You can read more in RISC-V-Reader. (A Chinese version is provided in this repository)

```text
31           25 24         20 19         15 14 12 11          7 6       0
+--------------+-------------+-------------+-----+-------------+---------+
|                    imm[31:12]                  |     rd      | 0110111 | U lui
|                    imm[31:12]                  |     rd      | 0010111 | U auipc
|              imm[20|10:1|11|19:12]             |     rd      | 1101111 | J jal
|         imm[11:0]          |     rs1     | 000 |     rd      | 1100111 | I jalr
| imm[12|10:5] |     rs2     |     rs1     | 000 | imm[4:1|11] | 1100011 | B beq
| imm[12|10:5] |     rs2     |     rs1     | 001 | imm[4:1|11] | 1100011 | B bne
| imm[12|10:5] |     rs2     |     rs1     | 100 | imm[4:1|11] | 1100011 | B blt
| imm[12|10:5] |     rs2     |     rs1     | 101 | imm[4:1|11] | 1100011 | B bge
| imm[12|10:5] |     rs2     |     rs1     | 110 | imm[4:1|11] | 1100011 | B bltu
| imm[12|10:5] |     rs2     |     rs1     | 111 | imm[4:1|11] | 1100011 | B bgeu
|          imm[11:0]         |     rs1     | 000 |     rd      | 0000011 | I lb
|          imm[11:0]         |     rs1     | 001 |     rd      | 0000011 | I lh
|          imm[11:0]         |     rs1     | 010 |     rd      | 0000011 | I lw
|          imm[11:0]         |     rs1     | 100 |     rd      | 0000011 | I lbu
|          imm[11:0]         |     rs1     | 101 |     rd      | 0000011 | I lhu
|  imm[11:5]   |     rs2     |     rs1     | 000 |  imm[4:0]   | 0100011 | S sb
|  imm[11:5]   |     rs2     |     rs1     | 001 |  imm[4:0]   | 0100011 | S sh
|  imm[11:5]   |     rs2     |     rs1     | 010 |  imm[4:0]   | 0100011 | S sw
|          imm[11:0]         |     rs1     | 000 |     rd      | 0010011 | I addi
|          imm[11:0]         |     rs1     | 010 |     rd      | 0010011 | I slti
|          imm[11:0]         |     rs1     | 011 |     rd      | 0010011 | I sltiu
|          imm[11:0]         |     rs1     | 100 |     rd      | 0010011 | I xori
|          imm[11:0]         |     rs1     | 110 |     rd      | 0010011 | I ori
|          imm[11:0]         |     rs1     | 111 |     rd      | 0010011 | I andi
|   0000000    |    shamt    |     rs1     | 001 |     rd      | 0010011 | I slli
|   0000000    |    shamt    |     rs1     | 101 |     rd      | 0010011 | I srli
|   0100000    |    shamt    |     rs1     | 101 |     rd      | 0010011 | I srai
|   0000000    |     rs2     |     rs1     | 000 |     rd      | 0110011 | R add
|   0100000    |     rs2     |     rs1     | 000 |     rd      | 0110011 | R sub
|   0000000    |     rs2     |     rs1     | 001 |     rd      | 0110011 | R sll
|   0000000    |     rs2     |     rs1     | 010 |     rd      | 0110011 | R slt
|   0000000    |     rs2     |     rs1     | 011 |     rd      | 0110011 | R sltu
|   0000000    |     rs2     |     rs1     | 100 |     rd      | 0110011 | R xor
|   0000000    |     rs2     |     rs1     | 101 |     rd      | 0110011 | R srl
|   0100000    |     rs2     |     rs1     | 101 |     rd      | 0110011 | R sra
|   0000000    |     rs2     |     rs1     | 110 |     rd      | 0110011 | R or
|   0000000    |     rs2     |     rs1     | 111 |     rd      | 0110011 | R and
+--------------+-------------+-------------+-----+-------------+---------+
```

## Five-Stage Pipeline Structure

The following picture is from slides of TA.

![five-stage pipeline structure](/five-stagePipeline.png)

## Performance

|   Test Case    |        CPU Clock        | Branch Prediction Correct Rate |
|:--------------:|:-----------------------:|:------------------------------:|
|  array_test1   |           298           |             47.83%             |
|  array_test2   |           351           |             44.44%             |
|   basicopt1    |         652276          |             99.21%             |
|   bulgarian    |         583570          |             94.66%             |
|      expr      |           616           |             82.14%             |
|      gcd       |           676           |             67.77%             |
|     hanoi      |         298519          |             98.28%             |
|    lvalue2     |           73            |             57.14%             |
|     magic      |         914959          |             82.77%             |
| manyarguments  |           83            |             72.73%             |
|   multiarray   |          2383           |             77.78%             |
|     naive      |           41            |              N/A               |
|       pi       |        119190379        |             85.35%             |
|     qsort      |         2028943         |             94.56%             |
|     queens     |         924950          |             80.97%             |
| statement_test |          1505           |             62.87%             |
|   superloop    |         567936          |             95.88%             |
|      tak       |         3148702         |             78.53%             |

## How to Use

Compile the program using Cmake at the path of the repository and run:

```Cmake
cmake -B build
cmake --build build
./build/code
```

## Contributor

Geral Yuan major in SJTU Computer Science.

[^1]: Quoted from Wikipedia, [RISC-V](https://en.wikipedia.org/wiki/RISC-V).
