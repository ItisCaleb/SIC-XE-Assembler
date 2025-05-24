#include "sic_parser.h"
#include "sic_table.h"
#include <vector>
#include <fmt/core.h>

SICParser *SICParser::instance() {
    if (inst == nullptr) {
        inst = new SICParser();
    }
    return inst;
}

SICInstruction SICParser::tokenize(std::string_view line, int line_num) {
    SICInstruction inst;
    inst.line_num = line_num;
    if (line.length() == 0 || line[0] == '.' || line[0] == '\n') {
        inst.should_ignore = true;
        return inst;
    }

    static const char *delim = " \t\n";
    line.remove_prefix(std::min(line.find_first_not_of(delim), line.size()));
    line.remove_suffix(line.length() - line.find_last_not_of(delim) - 1);
    size_t index = 0;
    std::vector<std::string_view> tokens;
    while ((index = line.find_first_of(delim)) != line.npos) {
        /* split instruction */
        tokens.push_back(line.substr(0, index));
        line = line.substr(index + 1);
        line.remove_prefix(
            std::min(line.find_first_not_of(delim), line.size()));
    }
    tokens.push_back(line);

    SICTable *table = SICTable::instance();

    if (tokens.size() == 1) {
        if (table->is_sic(tokens[0])) {
            inst.op_dir = tokens[0];
        } else {
            fmt::println(
                "Error: Token '{}' is not a valid SIC opcode/directive at line "
                "{}",
                tokens[0], line_num);
        }
    } else if (tokens.size() == 2) {
        if (table->is_sic(tokens[0])) {
            inst.op_dir = tokens[0];
            int index = tokens[1].find_first_of(",");
            inst.arg1 = tokens[1].substr(0, index);
            if (index != tokens[1].npos) {
                inst.arg2 = tokens[1].substr(index + 1);
            }
        } else if (table->is_sic(tokens[1])) {
            inst.symbol = tokens[0];
            inst.op_dir = tokens[1];
        } else {
            fmt::println(
                "Error: Token '{}' is not a valid SIC opcode/directive at line "
                "{}",
                tokens[1], line_num);
        }
    } else if (tokens.size() == 3) {
        if (table->is_sic(tokens[1])) {
            inst.symbol = tokens[0];
            inst.op_dir = tokens[1];
            int index = tokens[2].find_first_of(",");
            inst.arg1 = tokens[2].substr(0, index);
            if (index != tokens[2].npos) {
                inst.arg2 = tokens[2].substr(index + 1);
            }
        } else {
            fmt::println(
                "Error: Token '{}' is not a valid SIC opcode/directive at line "
                "{}",
                tokens[1], line_num);
        }
    }
    inst.is_imm = true;
    inst.is_indirect = true;
    /* extended mode */
    if (inst.op_dir[0] == '+') {
        inst.op_dir.remove_prefix(1);
        inst.is_extended = true;
    }
    /* indirect mode */
    if (inst.arg1.length() > 1 && inst.arg1[0] == '@') {
        inst.arg1.remove_prefix(1);
        inst.is_indirect = true;
        inst.is_imm = false;
    }
    /* immediate mode */
    if (inst.arg1.length() > 1 && inst.arg1[0] == '#') {
        inst.arg1.remove_prefix(1);
        inst.is_imm = true;
        inst.is_indirect = false;
    }
    /* indexing mode */
    if (inst.arg2 == "X") {
        if (inst.is_imm != inst.is_indirect) {
            fmt::println(
                "Error: Can't use Indexing addressing with Immediate or "
                "Indirect at line "
                "{}",
                line_num);
            return {};
        }
        inst.is_indexing = true;
    }
    return inst;
}

bool SICParser::tokenize_all(std::string_view asm_view,
                             std::vector<SICInstruction> &instructions) {
    int index = 0;
    int line_cnt = 1;
    SICInstruction inst;
    while ((index = asm_view.find_first_of("\n")) != asm_view.npos) {
        inst = tokenize(asm_view.substr(0, index), line_cnt);
        if (!inst.should_ignore) {
            if (inst.op_dir == "") return false;
            instructions.push_back(inst);
        }
        asm_view = asm_view.substr(index + 1);
        line_cnt++;
    }
    inst = tokenize(asm_view, line_cnt);
    if (!inst.should_ignore) {
        if (inst.op_dir == "") return false;
        instructions.push_back(inst);
    }
    return true;
}