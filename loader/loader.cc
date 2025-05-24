#include <fmt/format.h>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include "common.h"
#include "sic_program.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        fmt::println("Usage: {} <start address> <obj1> [<obj2>]", argv[0]);
        return 1;
    }
    uint32_t base;
    if (auto result = stoi(argv[1], 16)) {
        base = result.value;
        fmt::println("Start address: 0x{:x}", base);
    } else {
        fmt::println("Usage: {} <start address> <obj1> [<obj2>]", argv[0]);
        fmt::println("Error: Program base invalid.", argv[0]);
        return 1;
    }
    SICProgram program(base);

    for (int i = 2; i < argc; i++) {
        program.load_ref(argv[i]);
    }

    program.link();
    program.dump_memory();
}