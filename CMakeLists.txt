cmake_minimum_required(VERSION 3.16)
project(RISC_V)

set(CMAKE_CXX_STANDARD 17)

add_executable(code ./src/main.cpp
                    ./src/ALU.cpp
                    ./src/cpu.cpp
                    ./src/decoder.cpp
                    ./src/memory.cpp)