#include <string>	// std::string
#include <vector>	// std::vector
#include <sstream>	// std::stringstream
#include <iostream>	// std::iostream DEBUG MODE ON

#include "./types.h"
#include "./code_block.h"
#include "./state.h"
#include "./expression.h"

literal_expression_t::literal_expression_t(value_t value_): value(value_) { }

std::vector<std::string> literal_expression_t::evaluate(code_block_t* code_block, translator_state* state) {
	std::vector<std::string> result;

	std::stringstream ss;
	ss << "let ";
	std::string temp_var_name = state->get_next_temp_var_name();
	ss << temp_var_name << " ";

	switch (value.var_type) {
		case vt_int:
			ss << value.val_int;
			break;
		case vt_boolean:
			ss << value.val_boolean;
			break;		
	}

	result.push_back(ss.str());

	return result;
}

binary_expression_t::binary_expression_t(op_type operation_type_, expression_t* arg1_, expression_t* arg2_):
	operation_type(operation_type_), 
	arg1(arg1_), 
	arg2(arg2_) { }

std::vector<std::string> binary_expression_t::evaluate(code_block_t* code_block, translator_state* state) {
	std::vector<std::string> result;

	std::stringstream ss;
	switch (operation_type) {
		case op_add:
			ss << "add ";
			break;
		case op_sub:
			ss << "sub ";
			break;
		case op_mul:
			ss << "mul ";
			break;
		case op_div:
			ss << "div ";
			break;
	}

	std::vector<std::string> r1_str = arg1->evaluate(code_block, state);
	int left_expr_var_index = state->get_index();
	std::vector<std::string> r2_str = arg2->evaluate(code_block, state);
	int right_expr_var_index = state->get_index();
	
	for (int i = 0; i < r1_str.size(); ++i) {
		result.push_back(r1_str[i]);
	}
	for (int i = 0; i < r2_str.size(); ++i) {
		result.push_back(r2_str[i]);
	}

	std::string left_expr_var_name = state->generate_temp_var_name_with_index(left_expr_var_index);
	std::string right_expr_var_name = state->generate_temp_var_name_with_index(right_expr_var_index);
	
	ss << left_expr_var_name << " " << right_expr_var_name << " " << state->get_next_temp_var_name();
	
	result.push_back(ss.str());

	return result;
}

var_ref_expression_t::var_ref_expression_t(std::string name_): name(name_) { }

std::vector<std::string> var_ref_expression_t::evaluate(code_block_t* code_block, translator_state* tr_state) {
	std::vector<std::string> result;
	std::stringstream ss;

	std::string temp_var_name = code_block->get_temp_var_name(name);
	ss << "mov " << temp_var_name << " " << tr_state->get_next_temp_var_name();
	result.push_back(ss.str());

	return result;		
}

