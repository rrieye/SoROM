cmake_minimum_required(VERSION 3.10)
project(GF2_251_Arithmetic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(code)

set(COMMON_SOURCES 
    code/main.cpp 
    code/tests.cpp 
    code/benchmarks.cpp
)

add_executable(lab3_classic ${COMMON_SOURCES} code/FieldElement251.cpp)
target_compile_options(lab3_classic PRIVATE -O3)

add_executable(lab3_opt ${COMMON_SOURCES} code/FieldElement251_opt.cpp)
target_compile_options(lab3_opt PRIVATE -O3 -march=native)