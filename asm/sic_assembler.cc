#include "sic_assembler.h"
#include "sic_table.h"
#include <sstream>
#include <fmt/core.h>
#include <charconv>

SICAssembler *SICAssembler::instance() {
    if (inst == nullptr) {
        inst = new SICAssembler();
    }
    return inst;
}

static int stoi(std::string_view s, int base = 10) {
    int result_num;
    auto result =  std::from_chars(s.data(), s.data() + s.size(), result_num, base);
    if (result.ec == std::errc::invalid_argument) {
        return 0;
    }
    return result_num;
}

void SICAssembler::handle_symbol(AssembleContext &ctx) {
    int cur_loc = 0;
    SICTable *table = SICTable::instance();
    for (auto &inst : ctx.instructions) {
        if (inst.op_dir == "") {
            continue;
        }
        if (inst.op_dir == "START") {
            ctx.start_addr = stoi(inst.arg1, 16);
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
        if (table->is_instruction(inst.op_dir)) {
            SICOpcode op_code = table->get_opcode(inst.op_dir);
            if(op_code.is_xe) ctx.is_xe = true;
            switch (op_code.format) {
                case SICOpcodeType::FORMAT1:
                    cur_loc += 1;
                    break;
                case SICOpcodeType::FORMAT2:
                    cur_loc += 2;
                    break;
                case SICOpcodeType::FORMAT34:
                    if (inst.is_extended) {
                        cur_loc += 4;
                    } else {
                        cur_loc += 3;
                    }
                    break;
            }
        } else if (inst.op_dir == "WORD") {
            cur_loc += 3;
        } else if (inst.op_dir == "RESW") {
            cur_loc += stoi(inst.arg1) * 3;
        } else if (inst.op_dir == "RESB") {
            cur_loc += stoi(inst.arg1);
        } else if (inst.op_dir == "BYTE") {
            if (inst.arg1[0] == 'C') {
                cur_loc += inst.arg1.length() - 3;
            } else if (inst.arg1[0] == 'X') {
                cur_loc += (inst.arg1.length() - 3) / 2;
            }
        }
    }
}

std::string SICAssembler::gen_instruction(SICInstruction &inst,
                                          AssembleContext &ctx) {
    SICTable *table = SICTable::instance();
    SICOpcode &opcode = table->get_opcode(inst.op_dir);
    uint32_t m_code = opcode.opcode;
    std::string instruction;
    switch (opcode.format) {
        case SICOpcodeType::FORMAT1:
            instruction = fmt::format("{:02X}", m_code);
            break;
        case SICOpcodeType::FORMAT2: {
            m_code <<= 8;
            uint8_t r1 = table->get_regnum(inst.arg1);
            uint8_t r2 = table->get_regnum(inst.arg2);
            if (inst.op_dir == "SHIFTL" || inst.op_dir == "SHIFTR") {
                r2 = stoi(inst.arg2);
            }
            m_code |= r1 << 4;
            m_code |= r2;
            instruction = fmt::format("{:04X}", m_code);
            break;
        }
        case SICOpcodeType::FORMAT34:
            m_code <<= 16;
            if (ctx.is_xe && inst.is_imm) {
                m_code |= SICAddressingFlag::IMMEDIATE;
            }
            if (ctx.is_xe && inst.is_indirect) {
                m_code |= SICAddressingFlag::INDIRECT;
            }
            if (inst.arg1 != "") {
                if (inst.is_indexing) {
                    m_code |= SICAddressingFlag::INDEXING;
                }
                auto iter = ctx.sym_table.find(inst.arg1);
                if (iter != ctx.sym_table.end()) {
                    int num = iter->second;
                    int offset = num - ctx.cur_loc;
                    if (ctx.is_xe && offset >= -2048 && offset <= 2047) {
                        m_code |= SICAddressingFlag::PC;
                        m_code |= (offset - 3) & 0xfff;
                    } else if (ctx.is_xe && ctx.base_loc > -1 && 0 <= (num - ctx.base_loc) &&
                               (num - ctx.base_loc) <= 4095) {
                        m_code |= SICAddressingFlag::BASE;
                        m_code += num - ctx.base_loc;
                    } else if(ctx.is_xe && inst.is_extended){
                        m_code |= SICAddressingFlag::EXTENDED;
                        m_code <<= 8;
                        m_code += num;
                        ctx.relocation_locs.push_back(ctx.cur_loc);
                    } else if(!ctx.is_xe){
                        /* SIC mode */
                        m_code += num;
                    } else{
                        fmt::println("Error: Invalid argument '{}' at line {}",
                                     inst.arg1, inst.line_num);
                        return "";
                    }
                } else{
                    if (!ctx.is_xe){
                        fmt::println("Error: Undefined symbol '{}' at line {}",
                            inst.arg1, inst.line_num);
                        return "";
                    }
                    if (inst.is_extended) {
                        m_code |= SICAddressingFlag::EXTENDED;
                        m_code <<= 8;
                    }
                    m_code += stoi(inst.arg1);
                }
            }
            if (inst.is_extended) {
                instruction = fmt::format("{:08X}", m_code);
            } else {
                instruction = fmt::format("{:06X}", m_code);
            }
            break;
    }

    return instruction;
}

void SICAssembler::write_text(const std::string &text, int inst_len,
                              AssembleContext &ctx) {
    if ((ctx.cur_loc + inst_len - ctx.last_start) > 30 || ctx.res_flag) {
        ctx.result += fmt::format("T{:06X}{:02X}{}\n", ctx.last_start,
                                  ctx.code_cache.length() / 2, ctx.code_cache);
        ctx.last_start = ctx.cur_loc;
        ctx.code_cache = text;
    } else {
        ctx.code_cache += text;
    }
    ctx.res_flag = false;
    ctx.cur_loc += inst_len;
}

std::string SICAssembler::assemble(std::vector<SICInstruction> &instructions) {
    AssembleContext ctx(instructions);
    SICTable *table = SICTable::instance();
    handle_symbol(ctx);
    ctx.result = fmt::format("H{:6s}{:06X}{:06X}\n", ctx.program_name,
                             ctx.start_addr, ctx.program_length);
    ctx.cur_loc = ctx.start_addr;
    ctx.last_start = ctx.start_addr;
    for (auto &inst : instructions) {
        if (inst.op_dir == "START") continue;
        if (inst.op_dir == "END") {
            if (ctx.code_cache.length() > 0) {
                ctx.res_flag = true;
                write_text("", 0, ctx);
            }
            int addr = ctx.start_addr;
            if (inst.arg1.length() > 0) {
                addr = ctx.sym_table[inst.arg1];
            }
            if(ctx.is_xe){
                for(int loc : ctx.relocation_locs){
                    ctx.result += fmt::format("M{:06X}05\n", loc + 1);
                }
            }
            ctx.result += fmt::format("E{:06X}\n", addr);
            break;
        }
        if (table->is_instruction(inst.op_dir)) {
            std::string m_code = gen_instruction(inst, ctx);
            if (m_code == "") {
                fmt::println("Error: Undefined symbol '{}' at line {}",
                             inst.arg1, inst.line_num);
                return "";
            }
            write_text(m_code, m_code.length() / 2, ctx);
        } else if (inst.op_dir == "WORD") {
            std::string contant = fmt::format("{:06X}", stoi(inst.arg1));
            write_text(contant, 3, ctx);
        } else if (inst.op_dir == "BYTE") {
            int arg_len = 0;
            std::string constant;
            if (inst.arg1[0] == 'X') {
                arg_len = (inst.arg1.length() - 3) / 2;
                constant = inst.arg1.substr(2, arg_len * 2);
            } else if (inst.arg1[0] == 'C') {
                arg_len = inst.arg1.length() - 3;
                std::string_view tmp = inst.arg1.substr(2, arg_len);
                for (char c : tmp) {
                    constant += fmt::format("{:02X}", c);
                }
            }
            write_text(constant, arg_len, ctx);

        } else if (inst.op_dir == "RESB") {
            ctx.cur_loc += stoi(inst.arg1);
            ctx.res_flag = true;
        } else if (inst.op_dir == "RESW") {
            ctx.cur_loc += stoi(inst.arg1) * 3;
            ctx.res_flag = true;
        } else if (inst.op_dir == "BASE") {
            ctx.base_loc = ctx.sym_table[inst.arg1];
        }
    }
    return ctx.result;
}