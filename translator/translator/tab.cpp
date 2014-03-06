#include <cstring>
#include <cstdlib>
#include <iostream>

#include "bisonheader.h"

std::map<std::string, int> var_table;

void add_new_var(char* name, int value) {
	if (var_table.find(name) != var_table.end()) {
		std::cout << "Error: variable " << name << " was declared earlier" << std::endl;
		exit(1);
	}

	var_table.insert(std::make_pair(name, value));
}
