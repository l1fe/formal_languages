#ifndef _CLASS_H_INCLUDED_
#define _CLASS_H_INCLUDED_

#include <string>	// std::string
#include <vector>	// std::vector

#include "./statement.h"
#include "./code_block.h"
#include "./statement.h"
#include "./state.h"

class class_t {
public:
	class_t();

	void add_statement(statement_t* stmt);
	void write(std::ostream&, std::vector<std::string>&);
	void run();
private:
	code_block_t* main_block;
	std::string name;
	translator_state tr_state;
	std::vector<statement_t*> stmts;
};

#endif /* _CLASS_H_INCLUDED_ */

