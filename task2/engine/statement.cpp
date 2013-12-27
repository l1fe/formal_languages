#include <vector>	// std::vector
#include <string>	// std::string
#include <sstream>	// std::stringstream

#include <iostream>

#include "./types.h"
#include "./statement.h"
#include "./state.h"
#include "./code_block.h"

declaration_statement_t::declaration_statement_t(std::string var_name_, v_type var_type_): 
	var_name(var_name_), 
	var_type(var_type_) { }

std::vector<std::string> declaration_statement_t::execute(code_block_t* current_block, translator_state* state) {
	std::vector<std::string> result;

	std::stringstream ss;
	std::string temp_var_name = state->get_next_temp_var_name();
		
	ss << "let " << temp_var_name << " " << 0;
	current_block->set_real_var_name(var_name, temp_var_name);

	result.push_back(ss.str());
	
	return result;
}

assignment_statement_t::assignment_statement_t(std::string var_name_, expression_t* expr_):
	var_name(var_name_),
	expr(expr_) { }

std::vector<std::string> assignment_statement_t::execute(code_block_t* current_block, translator_state* state) {
	std::vector<std::string> result = expr->evaluate(current_block, state);

	std::string expr_result_var_name = state->get_current_temp_var_name();

	std::string temp_var_name = current_block->get_temp_var_name(var_name);

	std::stringstream ss;
	ss << "mov " << expr_result_var_name << " " << temp_var_name;

	result.push_back(ss.str());

	return result;
}

print_statement_t::print_statement_t(expression_t* expr_): expr(expr_) { }

std::vector<std::string> print_statement_t::execute(code_block_t* current_block, translator_state* state) {
	std::vector<std::string> expr_result = expr->evaluate(current_block, state);
	
	std::stringstream ss;
	ss << "out " << state->get_current_temp_var_name();
	
	expr_result.push_back(ss.str());
	
	return expr_result;
}

