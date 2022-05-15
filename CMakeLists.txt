project(ASM_TO_HEX)
cmake_minimum_required(VERSION 3.7)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(run.exe test_run/TEST.cpp lib/asm_reader.cpp)
