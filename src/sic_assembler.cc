#include "sic_assembler.h"
#include "sic_table.h"
#include <sstream>
#include <fmt/core.h>

SICAssembler *SICAssembler::instance() {
    if (inst == nullptr) {
        inst = new SICAssembler();
    }
    return inst;
}

void SICAssembler::handle_symbol(AssembleContext &ctx) {
    int cur_loc = 0;
    std::stringstream ss;
    SICTable *table = SICTable::instance();
    for (auto &inst : ctx.instructions) {
        if (inst.op_dir == "") {
            continue;
        }
        if (inst.op_dir == "START") {
            ss << std::hex << inst.arg1;
            ss >> ctx.start_addr;
            cur_loc = ctx.start_addr;
        }
        if (inst.op_dir == "END") {
            ctx.program_length = (cur_loc - ctx.start_addr);
            break;
        }
        if (inst.symbol != "") {
            if (ctx.sym_table.find(inst.symbol) != ctx.sym_table.end()) {
                fmt::println("Error: Symbol '{}' redifinition at line {}",
                             inst.symbol, inst.line_num);
                continue;
            }
            ctx.sym_table[inst.symbol] = cur_loc;
            if (inst.op_dir == "START") {
                ctx.program_name = inst.symbol;
            }
        }
        int num;
        if (table->is_instruction(inst.op_dir) || inst.op_dir == "WORD") {
            cur_loc += 3;
        } else if (inst.op_dir == "RESW") {
            ss << std::dec << inst.arg1;
            ss >> num;
            cur_loc += num * 3;
        } else if (inst.op_dir == "RESB") {
            ss << std::dec << inst.arg1;
            ss >> num;
            cur_loc += num;
        } else if (inst.op_dir == "BYTE") {
            if (inst.arg1[0] == 'C') {
                cur_loc += inst.arg1.length() - 3;
            } else if (inst.arg1[0] == 'X') {
                cur_loc += (inst.arg1.length() - 3) / 2;
            }
        }
        ss.clear();
    }
}

std::string SICAssembler::gen_instruction(SICInstruction &inst, AssembleContext &ctx){
    SICTable *table = SICTable::instance();
    unsigned int m_code = table->get_opcode(inst.op_dir) << 16;
    if(inst.arg1 != ""){
        if (inst.arg1.find(",X") == inst.arg1.length() - 2){
            m_code |= 1<<15;
            inst.arg1.remove_suffix(2);
        }
        if((ctx.sym_table.find(inst.arg1) != ctx.sym_table.end())){
            m_code += ctx.sym_table[inst.arg1];
        }else{
            return "";
        }
    }
    return fmt::format("{:06X}", m_code);
}

void SICAssembler::write_text(const std::string &text, int inst_len, AssembleContext &ctx){
    if((ctx.cur_loc + 3 - ctx.last_start) > 30 || ctx.res_flag){
        ctx.result += fmt::format("T{:06X}{:02X}{}\n", ctx.last_start, ctx.code_cache.length()/2, ctx.code_cache);
        ctx.last_start = ctx.cur_loc;
        ctx.code_cache = text;
    }else{
        ctx.code_cache += text;
    }
    ctx.res_flag = false;
    ctx.cur_loc += inst_len;
}

std::string SICAssembler::assemble(std::vector<SICInstruction> &instructions) {
    AssembleContext ctx(instructions);
    SICTable *table = SICTable::instance();
    std::stringstream ss;
    handle_symbol(ctx);
    ctx.result = fmt::format("H{:6s}{:06X}{:06X}\n", ctx.program_name, ctx.start_addr, ctx.program_length);
    ctx.cur_loc = ctx.start_addr;
    ctx.last_start = ctx.start_addr;
    for (auto &inst : instructions) {
        if (inst.op_dir == "START") continue;
        if (inst.op_dir == "END") {
            if(ctx.code_cache.length() > 0){
                ctx.res_flag = true;
                write_text("", 0, ctx);
            }
            int addr = ctx.start_addr;
            if(inst.arg1.length() > 0){
                addr = ctx.sym_table[inst.arg1];
            }
            ctx.result += fmt::format("E{:06X}\n", addr);
            break;
        }
        if (table->is_instruction(inst.op_dir)){
            std::string m_code = gen_instruction(inst, ctx);
            if(m_code == ""){
                fmt::println("Error: Undefined symbol '{}' at line {}", inst.arg1, inst.line_num);
                return "";
            }
            write_text(m_code, 3, ctx);
        }else if(inst.op_dir == "WORD"){
            int num;
            ss<<inst.arg1;
            ss>>num;
            std::string contant = fmt::format("{:06X}", num);
            write_text(contant, 3, ctx);
        }else if(inst.op_dir == "BYTE"){
            int arg_len = 0;
            std::string constant;
            if(inst.arg1[0] == 'X'){
                arg_len = (inst.arg1.length() - 3)/2;
                constant = inst.arg1.substr(2, arg_len * 2);
            }else if(inst.arg1[0] == 'C'){
                arg_len = inst.arg1.length() - 3;
                std::string_view tmp = inst.arg1.substr(2, arg_len);
                for(char c: tmp){
                    constant += fmt::format("{:02X}",c);
                }
            }
            write_text(constant, arg_len, ctx);
            
        }else if(inst.op_dir == "RESB"){
            int num;
            ss<<inst.arg1;
            ss>>num;
            ctx.cur_loc += num;
            ctx.res_flag = true;
        }else if(inst.op_dir == "RESW"){
            int num;
            ss<<inst.arg1;
            ss>>num;
            ctx.cur_loc += num * 3;
            ctx.res_flag = true;
        }
        ss.clear();
    }
    return ctx.result;
}