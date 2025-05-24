#include "sic_program.h"
#include "common.h"
#include <fmt/format.h>
#include <sstream>
bool SICProgram::load_ref(std::string_view obj_path) {
    std::ifstream f(obj_path);
    std::stringstream ss;
    if (f.fail()) {
        fmt::println("Cannot open object '{}'", obj_path);
        return false;
    }
    ss << f.rdbuf();
    f.close();
    SICObject &object = this->objects[obj_path];
    object.content = ss.str();
    std::string_view obj_view(object.content);
    object.name = trim(obj_view.substr(1, 6));
    uint32_t start_addr;
    uint32_t prog_len;
    if (auto res = stoi(obj_view.substr(7, 6), 16)) {
        start_addr = res.value;
    } else {
        fmt::println("Invalid start addr in header.", obj_path);
        return false;
    }
    if (auto res = stoi(obj_view.substr(13, 6), 16)) {
        prog_len = res.value;
    } else {
        fmt::println("Invalid program length in header.", obj_path);
        return false;
    }
    fmt::println("Name: {} Start: {} Len: {}", object.name, start_addr,
                 prog_len);
    this->memory_size += prog_len;
    this->ref_table[object.name] = code_addr;

    auto obj_lines = lines(obj_view);
    for (int i = 1; i < obj_lines.size(); i++) {
        if (obj_lines[i][0] != 'D') continue;
        auto line = obj_lines[i];
        int n = (line.length() - 1) / 12;
        for (int j = 0; j < n; j++) {
            std::string_view name = trim(line.substr(1 + 12 * j, 6));
            uint32_t addr;
            if (auto res = stoi(line.substr(7 + 12 * j, 6), 16)) {
                addr = res.value;
            } else {
                fmt::println("Invalid ref addr at {}:{}", obj_path, i);
            }
            this->ref_table[name] = code_addr + addr;
        }
    }
    object.code_addr = code_addr;
    code_addr += prog_len;
    return true;
}

bool SICProgram::parse_t_record(std::string_view line, uint32_t code_addr) {
    uint32_t addr;
    uint32_t len;
    if (auto res = stoi(line.substr(1, 6), 16)) {
        addr = res.value;
    } else {
        fmt::println("Invalie address.");
        return false;
    }
    if (auto res = stoi(line.substr(7, 2), 16)) {
        len = res.value;
    } else {
        fmt::println("Invalie length.");
        return false;
    }
    addr += code_addr - this->memory_base;
    addr *= 2;
    for (int i = 0; i < len * 2; i++) {
        this->memory[addr] = line[9 + i];
        addr++;
    }

    return true;
}
bool SICProgram::parse_m_record(std::string_view line, uint32_t code_addr) {
    uint32_t addr;
    uint32_t len;
    if (auto res = stoi(line.substr(1, 6), 16)) {
        addr = res.value;
    } else {
        fmt::println("Invalie address.");
        return false;
    }
    if (auto res = stoi(line.substr(7, 2), 16)) {
        len = res.value;
    } else {
        fmt::println("Invalie length.");
        return false;
    }
    addr += code_addr - this->memory_base;
    addr *= 2;
    if (len == 5) addr++;
    int32_t value;
    if (auto res = stoi(this->memory.substr(addr, len), 16)) {
        value = res.value;
    } else {
        fmt::println("Invalid length.");
        return false;
    }
    /* sign extension */
    int const mask = 1U << (len * 4 - 1);
    value = value & ((1U << len * 4) - 1);
    value = (value ^ mask) - mask;
    auto token = line.substr(10);
    if (line[9] == '+') {
        value += this->ref_table[token];
    } else {
        value -= this->ref_table[token];
    }
    std::string hex_v = (len == 5) ? fmt::format("{:05x}", value & 0xfffff)
                                   : fmt::format("{:06x}", value & 0xffffff);
    for (int i = 0; i < len; i++) {
        this->memory[addr + i] = hex_v[i];
    }
    return true;
}

bool SICProgram::link() {
    this->memory.resize(2 * this->memory_size);
    memset(this->memory.data(), '.', this->memory.length());
    for (auto &[path, obj] : this->objects) {
        auto obj_lines = lines(obj.content);
        for (int i = 1; i < obj_lines.size(); i++) {
            auto line = obj_lines[i];
            if (line[0] == 'T') {
                parse_t_record(line, obj.code_addr);
            }
            if (line[0] == 'M') {
                parse_m_record(line, obj.code_addr);
            }
        }
    }
    return true;
}

void SICProgram::dump_memory() {
    for (int i = 0; i < this->memory_size * 2; i += 2) {
        if (i % 32 == 0) {
            fmt::print("\n{:06x} ", this->memory_base + i / 2);
        }
        fmt::print("{} ", this->memory.substr(i, 2));
    }
}