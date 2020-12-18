cmake_minimum_required(VERSION 3.17)
project(AntiMoss)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -Werror")

add_executable(AntiMoss main.cpp minimizer.h minimizer.cpp obfuscator.h obfuscator.cpp)