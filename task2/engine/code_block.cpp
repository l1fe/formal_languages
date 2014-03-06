#include <string>		// std::string
#include <unordered_map>	// std::unordered_map
#include <iostream>

#include "./types.h"
#include "./code_block.h"

code_block_t::code_block_t(code_block_t* parent_ = NULL): parent(parent_) { }

bool code_block_t::declare_variable(std::string name, v_type type) {
	variable_t* var = get_variable_from_block(this, name);
	if (var == NULL) {
		var = new variable_t(name, type);
		declared_vars.insert(std::make_pair(name, var));
	
		return true;
	}

	return false;
}

variable_t* code_block_t::get_variable_from_block(code_block_t* block, std::string name) {
	if (block == NULL) {
		return NULL;
	}

	std::unordered_map<std::string, variable_t*>::iterator it = block->declared_vars.find(name);
	if (it == block->declared_vars.end()) {
		return get_variable_from_block(block->parent, name);
	}

	return it->second;
}

std::string code_block_t::get_temp_var_name(code_block_t* block, std::string& real_var_name) {
	if (block == NULL) {
		return std::string();
	}

	std::unordered_map<std::string, std::string>::iterator it = block->var_real_names.find(real_var_name);
	if (it == var_real_names.end()) {
		return get_temp_var_name(block->parent, real_var_name);	
	}

	return it->second;
}

void code_block_t::set_real_var_name(std::string real_var_name, std::string temp_var_name) {
	var_real_names.insert(std::make_pair(real_var_name, temp_var_name));
}
