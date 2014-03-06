#include <iostream>	// std::iostream
#include <cstdlib>
#include <sstream>

#include "bisonheader.h"

statement_list_t* Program;
size_t TEMP_VARS = 0;

std::string int_to_str(int number) {
	std::stringstream ss;
	ss << number;

	return ss.str();
}

std::string get_current_temp_var() {
	std::string temp_var_name = "tmp" + int_to_str(TEMP_VARS++);
	while (var_table.find(temp_var_name) != var_table.end()) {
		temp_var_name = "tmp" + int_to_str(TEMP_VARS++);
	}

	return temp_var_name;
}
