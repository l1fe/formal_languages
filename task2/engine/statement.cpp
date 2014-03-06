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
	
	state->line_number += result.size();

	return result;
}

assignment_statement_t::assignment_statement_t(std::string var_name_, expression_t* expr_):
	var_name(var_name_),
	expr(expr_) { }

std::vector<std::string> assignment_statement_t::execute(code_block_t* current_block, translator_state* state) {
	std::vector<std::string> result = expr->evaluate(current_block, state);

	std::string expr_result_var_name = state->get_current_temp_var_name();

	std::string temp_var_name = current_block->get_temp_var_name(current_block, var_name);

	std::stringstream ss;
	ss << "mov " << expr_result_var_name << " " << temp_var_name;

	result.push_back(ss.str());

	state->line_number += result.size();	

	return result;
}

print_statement_t::print_statement_t(expression_t* expr_): expr(expr_) { }

std::vector<std::string> print_statement_t::execute(code_block_t* current_block, translator_state* state) {
	std::vector<std::string> expr_result = expr->evaluate(current_block, state);
	
	std::stringstream ss;
	ss << "out " << state->get_current_temp_var_name();
	
	expr_result.push_back(ss.str());
	
	state->line_number += expr_result.size();

	return expr_result;
}

statement_list_t::statement_list_t() { }

std::vector<std::string> statement_list_t::execute(code_block_t* current_block, translator_state* state) {
	std::vector<std::string> result;

	for (int i = 0; i < stmts.size(); ++i) {
		std::vector<std::string> next_result = stmts[i]->execute(current_block, state);
		for (int j = 0; j < next_result.size(); ++j) {
			result.push_back(next_result[j]);
		}
	}

	return result;
}

void statement_list_t::add_statement(statement_t* stmt) {
	stmts.push_back(stmt);
}

code_block_statement_t::code_block_statement_t(statement_list_t* stmts_): stmts(stmts_) { }

std::vector<std::string> code_block_statement_t::execute(code_block_t* current_block, translator_state* state) {
	return stmts->execute(new code_block_t(current_block), state);
}

conditional_statement_t::conditional_statement_t(expression_t* expr_, statement_t* stmt_): expr(expr_), stmt(stmt_), sub_stmt(NULL) { }

conditional_statement_t::conditional_statement_t(expression_t* expr_, statement_t* stmt_, statement_t* sub_stmt_):
	expr(expr_),
	stmt(stmt_),
	sub_stmt(sub_stmt_) { }

std::vector<std::string> conditional_statement_t::execute(code_block_t* current_block, translator_state* state) {
	std::vector<std::string> result = expr->evaluate(current_block, state);
	state->line_number += result.size();

	int cmp_line_number = state->line_number + 2;

	std::stringstream ss;
	ss << "let zero 0";
	result.push_back(ss.str());
	ss.str(std::string());

	state->line_number += 2;

	std::string expr_res_temp_var_name = state->get_current_temp_var_name();

	std::vector<std::string> stmt_result = stmt->execute(current_block, state);

	std::vector<std::string> substmt_result;
	if (sub_stmt != NULL) {
		substmt_result = sub_stmt->execute(current_block, state);
	}

	int substmt_result_size = substmt_result.size();

	state->line_number += 1;
	state->line_number += substmt_result_size;

	ss << "cmp " << expr_res_temp_var_name << " zero " << cmp_line_number + 1 << " " << state->line_number + 2 - substmt_result_size << " " << cmp_line_number + 1;

	result.push_back(ss.str());
	
	for (int i = 0; i < stmt_result.size(); ++i) {
		result.push_back(stmt_result[i]);
	}
	
	ss.str(std::string());
	ss << "jmp " << state->line_number + 1;

	result.push_back(ss.str());

	for (int i = 0; i < substmt_result.size(); ++i) {
		result.push_back(substmt_result[i]);
	}

	return result;
}

