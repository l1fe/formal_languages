#include <iostream>
#include <sstream>

#include <stdlib.h>

#include "utils.h"
#include "expression.h"

void numeral_expression_t::generate_code(std::vector<std::string>& code) {
        result_variable = get_current_temp_var();
        code.push_back("let " + result_variable + " " + int_to_str(val));
}

void arithmetic_expression_t::generate_code(std::vector<std::string>& code) {
        e1->generate_code(code);
        if (e2 != NULL) {
                e2->generate_code(code);
        }

        result_variable = get_current_temp_var();

        std::string temp_var;

        switch(op) {
                case '+':
                        code.push_back("add " + e1->result_variable + " " + e2->result_variable + " " + result_variable);
                        break;
                case '-':
                        code.push_back("sub " + e1->result_variable + " " + e2->result_variable + " " +  result_variable);
                        break;
                case '*':
                        code.push_back("mul " + e1->result_variable + " " + e2->result_variable + " " + result_variable);
                        break;
                case '/':
                        code.push_back("div " + e1->result_variable + " " + e2->result_variable + " " + result_variable);
                        break;
                case '%':
                        temp_var = get_current_temp_var();
                        code.push_back("div " + e1->result_variable + " " + e2->result_variable + " " + temp_var);
                        code.push_back("mul " + temp_var + " " + e2->result_variable + " " + temp_var);
                        code.push_back("sub " + e1->result_variable + " " + temp_var + " " + result_variable);
                        break;
                case UMINUS:
                        temp_var = get_current_temp_var();
                        code.push_back("let " + temp_var + " 0");
                        code.push_back("sub " + temp_var + " " + e1->result_variable + " " + result_variable);
                        break;
        }
}

void variable_expression_t::generate_code(std::vector<std::string> &code) {
        if (var_table.find(name) == var_table.end()) {
                std::cerr << "Error: " << name << " not defined\n";
                exit(1);
        }

        result_variable = name;
}

void logic_expression_t::generate_code(std::vector<std::string>& code) {
        e1->generate_code(code);
        e2->generate_code(code);
}

std::string logic_expression_t::generate(size_t addr_if, size_t addr_else) {
        std::string result = "cmp " + e1->result_variable + " " + e2->result_variable + " ";

        if (op == "<") {
                result += int_to_str(addr_if) + " " + int_to_str(addr_else) + " " + int_to_str(addr_else);

                return result;
        }

        if (op == ">") {
                result += int_to_str(addr_else) + " " + int_to_str(addr_else) + " " + int_to_str(addr_if);

                return result;
        }

        if (op == "==") {
                result += int_to_str(addr_else) + " " + int_to_str(addr_if) + " " + int_to_str(addr_else);

                return result;
        }

        if (op == "!=") {
                result += int_to_str(addr_if) + " " + int_to_str(addr_else) + " " + int_to_str(addr_if);

                return result;
        }

        if (op == ">=") {
                result += int_to_str(addr_else) + " " + int_to_str(addr_if) + " " + int_to_str(addr_if);

                return result;
        }

        if (op == "<=") {
                result += int_to_str(addr_if) + " " + int_to_str(addr_if) + " " + int_to_str(addr_else);

                return result;
        }

        return result;
}




