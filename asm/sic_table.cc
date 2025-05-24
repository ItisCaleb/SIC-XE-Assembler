#include "sic_table.h"

void SICTable::init_op() {
    /* SIC */
    this->op_tab["ADD"] = SICOpcode{0x18, SICOpcodeType::FORMAT34, false};
    this->op_tab["AND"] = SICOpcode{0x40, SICOpcodeType::FORMAT34, false};
    this->op_tab["COMP"] = SICOpcode{0x28, SICOpcodeType::FORMAT34, false};
    this->op_tab["DIV"] = SICOpcode{0x24, SICOpcodeType::FORMAT34, false};
    this->op_tab["J"] = SICOpcode{0x3C, SICOpcodeType::FORMAT34, false};
    this->op_tab["JEQ"] = SICOpcode{0x30, SICOpcodeType::FORMAT34, false};
    this->op_tab["JGT"] = SICOpcode{0x34, SICOpcodeType::FORMAT34, false};
    this->op_tab["JLT"] = SICOpcode{0x38, SICOpcodeType::FORMAT34, false};
    this->op_tab["JSUB"] = SICOpcode{0x48, SICOpcodeType::FORMAT34, false};
    this->op_tab["LDA"] = SICOpcode{0x00, SICOpcodeType::FORMAT34, false};
    this->op_tab["LDCH"] = SICOpcode{0x50, SICOpcodeType::FORMAT34, false};
    this->op_tab["LDL"] = SICOpcode{0x08, SICOpcodeType::FORMAT34, false};
    this->op_tab["LDX"] = SICOpcode{0x04, SICOpcodeType::FORMAT34, false};
    this->op_tab["MUL"] = SICOpcode{0x20, SICOpcodeType::FORMAT34, false};
    this->op_tab["OR"] = SICOpcode{0x44, SICOpcodeType::FORMAT34, false};
    this->op_tab["RD"] = SICOpcode{0xD8, SICOpcodeType::FORMAT34, false};
    this->op_tab["RSUB"] = SICOpcode{0x4C, SICOpcodeType::FORMAT34, false};
    this->op_tab["STA"] = SICOpcode{0x0C, SICOpcodeType::FORMAT34, false};
    this->op_tab["STCH"] = SICOpcode{0x54, SICOpcodeType::FORMAT34, false};
    this->op_tab["STL"] = SICOpcode{0x14, SICOpcodeType::FORMAT34, false};
    this->op_tab["STSW"] = SICOpcode{0xE8, SICOpcodeType::FORMAT34, false};
    this->op_tab["STX"] = SICOpcode{0x10, SICOpcodeType::FORMAT34, false};
    this->op_tab["SUB"] = SICOpcode{0x1C, SICOpcodeType::FORMAT34, false};
    this->op_tab["TD"] = SICOpcode{0xE0, SICOpcodeType::FORMAT34, false};
    this->op_tab["TIX"] = SICOpcode{0x2C, SICOpcodeType::FORMAT34, false};
    this->op_tab["WD"] = SICOpcode{0xDC, SICOpcodeType::FORMAT34, false};

    /* XE */
    this->op_tab["ADDF"] = SICOpcode{0x58, SICOpcodeType::FORMAT34, true};
    this->op_tab["ADDR"] = SICOpcode{0x90, SICOpcodeType::FORMAT2, true};
    this->op_tab["CLEAR"] = SICOpcode{0xB4, SICOpcodeType::FORMAT2, true};
    this->op_tab["COMPF"] = SICOpcode{0x88, SICOpcodeType::FORMAT34, true};
    this->op_tab["COMPR"] = SICOpcode{0xA0, SICOpcodeType::FORMAT2, true};
    this->op_tab["DIVF"] = SICOpcode{0x64, SICOpcodeType::FORMAT34, true};
    this->op_tab["DIVR"] = SICOpcode{0x9C, SICOpcodeType::FORMAT2, true};
    this->op_tab["FIX"] = SICOpcode{0xC4, SICOpcodeType::FORMAT1, true};
    this->op_tab["FLOAT"] = SICOpcode{0xC0, SICOpcodeType::FORMAT1, true};
    this->op_tab["HIO"] = SICOpcode{0xF4, SICOpcodeType::FORMAT1, true};
    this->op_tab["LDB"] = SICOpcode{0x68, SICOpcodeType::FORMAT34, true};
    this->op_tab["LDF"] = SICOpcode{0x70, SICOpcodeType::FORMAT34, true};
    this->op_tab["LDS"] = SICOpcode{0x6C, SICOpcodeType::FORMAT34, true};
    this->op_tab["LDT"] = SICOpcode{0x74, SICOpcodeType::FORMAT34, true};
    this->op_tab["LPS"] = SICOpcode{0xD0, SICOpcodeType::FORMAT34, true};
    this->op_tab["MULF"] = SICOpcode{0x60, SICOpcodeType::FORMAT34, true};
    this->op_tab["MULR"] = SICOpcode{0x98, SICOpcodeType::FORMAT2, true};
    this->op_tab["NORM"] = SICOpcode{0xC8, SICOpcodeType::FORMAT1, true};
    this->op_tab["RMO"] = SICOpcode{0xAC, SICOpcodeType::FORMAT2, true};
    this->op_tab["SHIFTL"] = SICOpcode{0xA4, SICOpcodeType::FORMAT2, true};
    this->op_tab["SHIFTR"] = SICOpcode{0xA8, SICOpcodeType::FORMAT2, true};
    this->op_tab["SIO"] = SICOpcode{0xF0, SICOpcodeType::FORMAT1, true};
    this->op_tab["SSK"] = SICOpcode{0xEC, SICOpcodeType::FORMAT34, true};
    this->op_tab["STB"] = SICOpcode{0x78, SICOpcodeType::FORMAT34, true};
    this->op_tab["STF"] = SICOpcode{0x80, SICOpcodeType::FORMAT34, true};
    this->op_tab["STI"] = SICOpcode{0xDC, SICOpcodeType::FORMAT34, true};
    this->op_tab["STS"] = SICOpcode{0x14, SICOpcodeType::FORMAT34, true};
    this->op_tab["STT"] = SICOpcode{0x84, SICOpcodeType::FORMAT34, true};
    this->op_tab["SUBF"] = SICOpcode{0x5C, SICOpcodeType::FORMAT34, true};
    this->op_tab["SUBR"] = SICOpcode{0x94, SICOpcodeType::FORMAT2, true};
    this->op_tab["SVC"] = SICOpcode{0xB0, SICOpcodeType::FORMAT2, true};
    this->op_tab["TIO"] = SICOpcode{0xF8, SICOpcodeType::FORMAT1, true};
    this->op_tab["TIXR"] = SICOpcode{0xB8, SICOpcodeType::FORMAT2, true};
}
void SICTable::init_directive() {
    this->directive.insert("START");
    this->directive.insert("END");
    this->directive.insert("WORD");
    this->directive.insert("BYTE");
    this->directive.insert("RESW");
    this->directive.insert("RESB");
    this->directive.insert("BASE");
}

void SICTable::init_registers() {
    this->reg_tab["A"] = 0;
    this->reg_tab["X"] = 1;
    this->reg_tab["L"] = 2;
    this->reg_tab["PC"] = 8;
    this->reg_tab["SW"] = 9;
    this->reg_tab["B"] = 3;
    this->reg_tab["S"] = 4;
    this->reg_tab["T"] = 5;
    this->reg_tab["F"] = 6;
}

SICTable::SICTable() {
    init_op();
    init_directive();
    init_registers();
}

SICTable *SICTable::instance() {
    if (inst == nullptr) {
        inst = new SICTable();
    }
    return inst;
}
bool SICTable::is_instruction(std::string_view inst) {
    if (inst[0] == '+') inst.remove_prefix(1);
    return this->op_tab.find(inst) != this->op_tab.end();
}

SICOpcode &SICTable::get_opcode(std::string_view inst) {
    return this->op_tab[inst];
}

uint8_t SICTable::get_regnum(std::string_view reg) {
    return this->reg_tab[reg];
}

bool SICTable::is_directive(std::string_view dir) {
    return this->directive.find(dir) != this->directive.end();
}

bool SICTable::is_sic(std::string_view text) {
    return is_instruction(text) || is_directive(text);
}

bool SICTable::is_sicxe(std::string_view text) {
    auto iter = this->op_tab.find(text);
    if (iter == this->op_tab.end()) return false;
    return iter->second.is_xe;
}