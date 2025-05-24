cmake_minimum_required(VERSION 3.14)

project(SICAssembler
    VERSION 0.1
    DESCRIPTION "An SIC Assembler"
    LANGUAGES CXX C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

find_package(fmt CONFIG REQUIRED)
find_package(scn CONFIG REQUIRED)

add_subdirectory(asm)
add_subdirectory(loader)