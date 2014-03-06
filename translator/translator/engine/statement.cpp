#include <iostream>
#include <sstream>

#include <stdlib.h>

#include "expression.h"
#include "statement.h"
#include "utils.h"

void statement_list_t::generate(std::vector<std::string>& code) {
        for (std::vector<statement_t*>::iterator i = list.begin(); i < list.end(); i++) {
                (*i)->generate(code);
        }
}

void print_statement_t::generate(std::vector<std::string>& code) {
        e->generate_code(code);

        code.push_back("out " + e->result_variable);
}

void if_statement_t::generate(std::vector<std::string>& code) {
        e->generate_code(code);

        size_t if_position = code.size();
        size_t jmp_f_pos;

        code.push_back("");

        L1->generate(code);

        size_t else_position = code.size();

        if (L2 != NULL) {
                ++else_position;
                jmp_f_pos = code.size();
                code.push_back("");

                L2->generate(code);
        }
        code[if_position] = e->generate(if_position + 1, else_position);

        if (L2 != NULL) {
                code[jmp_f_pos] = "jmp " + int_to_str(code.size());
        }
}

void assign_statement_t::generate(std::vector<std::string>& code) {
        if (var_table.find(name) == var_table.end()) {
                std::cerr << "Runtime error: " << name << " undefined\n";
                exit(1);
        }

        numeral_expression_t* en = dynamic_cast<numeral_expression_t*>(e);
        if (en == NULL) {
                e->generate_code(code);
                code.push_back("mov " + e->result_variable + " " + name);
        } else {
                code.push_back("let " + name + " " + int_to_str(en->val));
        }
}

void while_statement_t::generate(std::vector<std::string>& code) {
        size_t start_position = code.size();
        e->generate_code(code);
        size_t conditional_position = code.size();
        code.push_back("");
        body->generate(code);
        code.push_back("jmp " + int_to_str(start_position));
        size_t exit_position = code.size();
        code[conditional_position] = e->generate(conditional_position + 1, exit_position);
}

void for_statement_t::generate(std::vector<std::string>& code) {
        prepare->generate(code);
        size_t start_position = code.size();
        condition->generate_code(code);
        size_t conditional_position = code.size();
        code.push_back("");
        body->generate(code);
        increment->generate(code);
        code.push_back("jmp " + int_to_str(start_position));

        size_t exit_position = code.size();
        code[conditional_position] = condition->generate(conditional_position + 1, exit_position);
}

std::string logic_complex_expression_t::generate(size_t addr_if, size_t addr_else) {
        return "cmp " + result_variable + " " + null_variable + " " + int_to_str(addr_else) + " " + int_to_str(addr_else) + " " + int_to_str(addr_if);
}

void logic_complex_expression_t::generate_code(std::vector<std::string>& code) {
        null_variable = get_current_temp_var();
        code.push_back("let " + null_variable + " 0");
        std::string e1_name = generate_logic_expression_t_to_bool(le1, code);
        std::string e2_name;

        if (le2 != NULL) {
                e2_name = generate_logic_expression_t_to_bool(le2, code);
        }

        result_variable = get_current_temp_var();
        size_t actualPos = 0;

        switch (op) {
                case AND:
                        code.push_back("mul " + e1_name + " " + e2_name + " " + result_variable);
                        break;
                case OR:
                        code.push_back("add " + e1_name + " " + e2_name + " " + result_variable);
                        actualPos = code.size();
                        code.push_back("cmp " + result_variable + " " + null_variable + " " +  int_to_str(actualPos + 3) + " " + int_to_str(actualPos + 3) + " " + int_to_str(actualPos + 2));
                        code.push_back("let " + result_variable + " 1");
                        break;
                case '!':
                        code.push_back("let " + result_variable + " 1");
                        code.push_back("sub " + result_variable + " " + e1_name + " " + result_variable);
                        break;
        }
}

std::string logic_complex_expression_t::generate_logic_expression_t_to_bool(logic_expression_t* e, std::vector<std::string>& code) {
        e->generate_code(code);

        std::string result = get_current_temp_var();
        code.push_back("let " + result + " 0");

        size_t if_position = code.size();
        code.push_back(e->generate(if_position + 1, if_position + 2));
        code.push_back("let " + result + " 1");

        return result;
}


