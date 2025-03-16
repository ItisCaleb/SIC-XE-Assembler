#include "sic_table.h"

void SICTable::init_op() {
    this->op_tab["ADD"] = 0x18;
    this->op_tab["AND"] = 0x40;
    this->op_tab["COMP"] = 0x28;
    this->op_tab["DIV"] = 0x24;
    this->op_tab["J"] = 0x3C;
    this->op_tab["JEQ"] = 0x30;
    this->op_tab["JGT"] = 0x34;
    this->op_tab["JLT"] = 0x38;
    this->op_tab["JSUB"] = 0x48;
    this->op_tab["LDA"] = 0x00;
    this->op_tab["LDCH"] = 0x50;
    this->op_tab["LDL"] = 0x08;
    this->op_tab["LDX"] = 0x04;
    this->op_tab["MUL"] = 0x20;
    this->op_tab["OR"] = 0x44;
    this->op_tab["RD"] = 0xD8;
    this->op_tab["RSUB"] = 0x4C;
    this->op_tab["STA"] = 0x0C;
    this->op_tab["STCH"] = 0x54;
    this->op_tab["STL"] = 0x14;
    this->op_tab["STSW"] = 0xE8;
    this->op_tab["STX"] = 0x10;
    this->op_tab["SUB"] = 0x1C;
    this->op_tab["TD"] = 0xE0;
    this->op_tab["TIX"] = 0x2C;
    this->op_tab["WD"] = 0xDC;
}
void SICTable::init_directive() {
    this->directive.insert("START");
    this->directive.insert("END");
    this->directive.insert("WORD");
    this->directive.insert("BYTE");
    this->directive.insert("RESW");
    this->directive.insert("RESB");
}

SICTable::SICTable() {
    init_op();
    init_directive();
}

SICTable *SICTable::instance() {
    if (inst == nullptr) {
        inst = new SICTable();
    }
    return inst;
}
bool SICTable::is_instruction(std::string_view inst) {
    return this->op_tab.find(inst) != this->op_tab.end();
}

unsigned int SICTable::get_opcode(std::string_view inst) { 
    auto iter = this->op_tab.find(inst);
    if(iter == this->op_tab.end()) return -1;
    return iter->second; 
}

bool SICTable::is_directive(std::string_view dir) {
    return this->directive.find(dir) != this->directive.end();
}

bool SICTable::is_sic(std::string_view text) {
    return is_instruction(text) || is_directive(text);
}