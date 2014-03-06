#ifndef _STATEMENT_H_INCLUDED_
#define _STATEMENT_H_INCLUDED_

#include <vector>	// std::vector
#include <string>	// std::string

#include "./types.h"
#include "./code_block.h"
#include "./state.h"
#include "./expression.h"

class statement_t {
public:
        virtual std::vector<std::string> execute(code_block_t*, translator_state*) = 0;
};

class statement_list_t : public statement_t {
public:
	statement_list_t();

	virtual std::vector<std::string> execute(code_block_t*, translator_state*);
	void add_statement(statement_t* stmt);
private:
	std::vector<statement_t*> stmts;
};

class declaration_statement_t : public statement_t {
public:
        declaration_statement_t(std::string, v_type);

        virtual std::vector<std::string> execute(code_block_t*, translator_state*);
private:
        std::string var_name;
        v_type var_type;
};

class assignment_statement_t : public statement_t {
public:
        assignment_statement_t(std::string, expression_t*);

        virtual std::vector<std::string> execute(code_block_t*, translator_state*);
private:
	std::string var_name;
        expression_t* expr;
};

class print_statement_t : public statement_t {
public:
	print_statement_t(expression_t*);

	virtual std::vector<std::string> execute(code_block_t*, translator_state*);
private:
	expression_t* expr;
};

class code_block_statement_t : public statement_t {
public:
	code_block_statement_t(statement_list_t*);

	virtual std::vector<std::string> execute(code_block_t*, translator_state*);
private:
	statement_list_t* stmts;
};

class conditional_statement_t : public statement_t {
public:
	conditional_statement_t(expression_t*, statement_t*);
	conditional_statement_t(expression_t*, statement_t*, statement_t*);
	
	virtual std::vector<std::string> execute(code_block_t*, translator_state*);
private:
	expression_t* expr;
	statement_t* stmt;
	statement_t* sub_stmt;
};

#endif /* _STATEMENT_H_INCLUDED */

