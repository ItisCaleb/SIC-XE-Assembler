#ifndef SIC_ASSEMBLER_H_
#define SIC_ASSEMBLER_H_
#include "sic_parser.h"
#include <map>

class SICAssembler {
   private:
    enum SICAddressingFlag : int {
        INDIRECT = 1 << 17,
        IMMEDIATE = 1 << 16,
        INDEXING = 1 << 15,
        BASE = 1 << 14,
        PC = 1 << 13,
        EXTENDED = 1 << 12
    };
    struct AssembleContext {
        std::vector<SICInstruction> &instructions;
        std::vector<int> relocation_locs;
        std::map<std::string_view, int> sym_table;
        std::string_view program_name;
        int program_length = 0;
        int start_addr = 0;

        bool res_flag = false;
        int last_start = 0;
        int cur_loc = 0;
        int base_loc = -1;
        bool is_xe = false;
        std::string code_cache;
        std::string result;
        AssembleContext(std::vector<SICInstruction> &insts)
            : instructions(insts) {}
    };
    inline static SICAssembler *inst = nullptr;
    std::string gen_instruction(SICInstruction &inst, AssembleContext &ctx);
    void write_text(const std::string &text, int inst_len,
                    AssembleContext &ctx);
    void handle_symbol(AssembleContext &ctx);

   public:
    static SICAssembler *instance();
    std::string assemble(std::vector<SICInstruction> &instructions);
};

#endif