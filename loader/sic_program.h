#ifndef SIC_PROGRAM_H_
#define SIC_PROGRAM_H_
#include <stdint.h>
#include <map>
#include <string>
#include <fstream>

struct SICObject{
    std::string content;
    std::string_view name;
    uint32_t code_addr = 0;
};

class SICProgram {
    uint32_t memory_base;
    uint32_t memory_size = 0;
    uint32_t code_addr = 0;
    std::map<std::string_view, SICObject> objects;
    std::map<std::string_view, uint32_t> ref_table;
    std::string memory;
    bool parse_t_record(std::string_view line, uint32_t code_addr);
    bool parse_m_record(std::string_view line, uint32_t code_addr);

   public:
    SICProgram(uint32_t mem_base) : memory_base(mem_base), code_addr(mem_base) {}
    bool load_ref(std::string_view obj_path);
    bool link();
    std::string& get_memory(){
        return this->memory;
    }
    void dump_memory();
};

#endif