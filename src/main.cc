#include "sic_parser.h"
#include "sic_assembler.h"
#include <fmt/core.h>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, char **argv) {
    if (argc < 2) {
        fmt::println("Usage: {} <asm file>", argv[0]);
        return 1;
    }
    std::ifstream f(argv[1]);
    std::stringstream ss;
    std::string asm_file;
    if (f.fail()) {
        fmt::println("Cannot open file '{}'", argv[1]);
        return 1;
    }
    ss << f.rdbuf();
    f.close();
    asm_file = ss.str();

    SICParser *parser = SICParser::instance();
    std::string_view asm_view(asm_file);
    std::vector<SICInstruction> instructions;
    bool res = parser->tokenize_all(asm_view, instructions);
    if (!res) {
        fmt::println("There are errors in instructions, stop processing.");
        return 1;
    }

    SICAssembler *assembler = SICAssembler::instance();
    std::string result = assembler->assemble(instructions);
    fmt::println("{}", result);

    return 0;
}