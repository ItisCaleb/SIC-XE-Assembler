#ifndef SIC_TABLE_H_
#define SIC_TABLE_H_
#include <map>
#include <set>
#include <string_view>

enum class SICOpcodeType{
    FORMAT1, FORMAT2, FORMAT34
};

struct SICOpcode{
    uint32_t opcode = 0;
    SICOpcodeType format;
    bool is_xe;
};

class SICTable {
   private:
    inline static SICTable *inst = nullptr;
    std::map<std::string_view, SICOpcode> op_tab;
    std::map<std::string_view, uint8_t> reg_tab;
    std::set<std::string_view> directive;
    void init_op();
    void init_directive();
    void init_registers();
    SICTable();

   public:
    static SICTable *instance();
    bool is_instruction(std::string_view inst);
    SICOpcode& get_opcode(std::string_view inst);
    uint8_t get_regnum(std::string_view reg);
    bool is_directive(std::string_view dir);
    bool is_sic(std::string_view text);
    bool is_sicxe(std::string_view text);
};

#endif