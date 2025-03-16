#ifndef SIC_TABLE_H_
#define SIC_TABLE_H_
#include <map>
#include <set>
#include <string_view>

class SICTable {
   private:
    inline static SICTable *inst = nullptr;
    std::map<std::string_view, int> op_tab;
    std::set<std::string_view> directive;
    void init_op();
    void init_directive();
    SICTable();

   public:
    static SICTable *instance();
    bool is_instruction(std::string_view inst);
    unsigned int get_opcode(std::string_view inst);
    bool is_directive(std::string_view dir);
    bool is_sic(std::string_view text);
};

#endif