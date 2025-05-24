#ifndef SIC_PARSER_H_
#define SIC_PARSER_H_
#include <string>
#include <string_view>

struct SICInstruction {
    std::string_view symbol;
    std::string_view op_dir;
    std::string_view arg1;
    std::string_view arg2;
    int line_num;
    bool should_ignore = false;
    bool is_extended = false;
    bool is_imm = false;
    bool is_indirect = false;
    bool is_indexing = false;
};

class SICParser {
   private:
    inline static SICParser *inst = nullptr;
    SICInstruction tokenize(std::string_view line, int line_num);

   public:
    static SICParser *instance();
    bool tokenize_all(std::string_view asm_view, std::vector<SICInstruction> &instructions);
};

#endif