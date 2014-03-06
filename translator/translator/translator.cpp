#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "bisonheader.h"

extern FILE* yyin;

void yyerror(const char* s) {
	std::cerr << "Fatal error: " << s << std::endl;
	exit(1);
}

int main(int argc, const char* argv[]) {
	if (argc != 3) {
		std::cerr << "Error: correct usage " << argv[0];
		std::cout << " <INPUT_FILE_NAME> <OUTPUT_FILE_NAME>" << std::endl;
		exit(1);
	}

	FILE* input_file;
	if (!(input_file = fopen(argv[1], "r"))) {
		std::cerr << "Error: can't open file " << argv[1] << std::endl;
		exit(1);
	}

	yyin = input_file;

	std::ofstream output(argv[2], std::ofstream::out);
	if (!output.is_open()) { 
		std::cout << "Error: can't open file " << std::endl;
		exit(1);
	}
	
	yyparse();

	std::vector<std::string> code;
	for (std::map<std::string, int>::iterator i = var_table.begin(); i != var_table.end(); ++i) {
		code.push_back("let " + i->first + " " + int_to_str(i->second));
	}

	Program->generate(code);

	for (std::vector<std::string>::iterator i = code.begin(); i != code.end(); ++i) {
		output << *i << std::endl;
	}

	output.close();

	return 0;
}

