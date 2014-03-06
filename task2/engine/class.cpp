#include <string>	// std::string
#include <vector>	// std::vector
#include <fstream>	// std::fstream::out
#include <iostream>

#include "./class.h"
#include "./code_block.h"

class_t::class_t() {
	main_block = new code_block_t(NULL);
}

void class_t::add_statement(statement_t* stmt) {
	stmts.push_back(stmt);
}

void class_t::write(std::ostream& output_stream, std::vector<std::string>& strings) {
	for (int i = 0; i < strings.size(); ++i) {
		output_stream << strings[i] << std::endl;
	}
}

void class_t::run() {
	std::fstream fs("jt_out.t", std::fstream::out);

	for (int i = 0; i < stmts.size(); ++i) {
		std::vector<std::string> stmt_result = stmts[i]->execute(main_block, &tr_state);
		write(fs, stmt_result);
	}

	fs << std::endl;

	std::cout << "line number = " << tr_state.line_number << std::endl; 
	fs.close();
}

