#ifndef _TRANSLATOR_H_INCLUDED_
#define _TRANSLATOR_H_INCLUDED_

#include <string>		// std::string
#include <vector>		// std::vector
#include <map>			// std::map

#include "statement.h"

extern statement_list_t* Program;

#include "../java.tab.hpp"

void add_new_var(char* name, int value);
std::string int_to_str(int number);
std::string get_current_temp_var();

extern std::map<std::string, int> var_table;

int yylex();
void yyerror(const char* s);

#endif /* _TRANSLATOR_H_INCLUDED_ */
