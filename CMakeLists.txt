cmake_minimum_required(VERSION 3.14)

project(SICAssembler
    VERSION 0.1
    DESCRIPTION "An SIC Assembler"
    LANGUAGES CXX C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt CONFIG REQUIRED)


add_subdirectory(src)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build")
