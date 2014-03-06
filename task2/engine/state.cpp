#include <string>	// std::string
#include <sstream>	// std::stringstream

#include "./state.h"

translator_state::translator_state(): temp_var_index(0), line_number(-1) { }

std::string translator_state::get_next_temp_var_name() {
	temp_var_index += 1;
	
	std::stringstream ss;
	ss << TEMP_VAR_NAME_PATTERN << temp_var_index;

	return ss.str();
}

int translator_state::get_index() {
	return temp_var_index;
}

std::string translator_state::generate_temp_var_name_with_index(int index) {
	std::stringstream ss;
	ss << TEMP_VAR_NAME_PATTERN;
	ss << index;

	return ss.str();
}

std::string translator_state::get_current_temp_var_name() {
	std::stringstream ss;
	ss << TEMP_VAR_NAME_PATTERN;
	ss << temp_var_index;

	return ss.str();
}

