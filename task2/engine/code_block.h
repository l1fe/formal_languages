#ifndef _CODE_BLOCK_H_INCLUDED_
#define _CODE_BLOCK_H_INCLUDED_

#include <unordered_map>	// std::unordered_map

#include "./types.h"

class code_block_t {
public:
        code_block_t(code_block_t*);

        bool declare_variable(std::string, v_type);
        variable_t* get_variable_from_block(code_block_t*, std::string);

        std::string get_temp_var_name(std::string&);
        void set_real_var_name(std::string, std::string);
private:
        code_block_t* parent;
        std::unordered_map<std::string, variable_t*> declared_vars;
        std::unordered_map<std::string, std::string> var_real_names;
};

#endif /* _CODE_BLOCK_H_INCLUDED_ */
