#ifndef _EXPRESSION_H_INCLUDED_
#define _EXPRESSION_H_INCLUDED_

#include <string>	// std::string
#include <vector>	// std::vector

#include "./state.h"
#include "./types.h"

class expression_t {
public:
        virtual std::vector<std::string> evaluate(code_block_t*, translator_state*) = 0;
};

// INT | BOOL
class literal_expression_t : public expression_t {
public:
        literal_expression_t(value_t);

        virtual std::vector<std::string> evaluate(code_block_t*, translator_state*);
private:
        value_t value;
};

// expr BINARY_OP expr
class binary_expression_t : public expression_t {
public:
        binary_expression_t(op_type, expression_t*, expression_t*);

        virtual std::vector<std::string> evaluate(code_block_t*, translator_state*);
private:
        op_type operation_type;
        expression_t* arg1;
        expression_t* arg2;
};

class var_ref_expression_t : public expression_t {
public:
        var_ref_expression_t(std::string);

        virtual std::vector<std::string> evaluate(code_block_t*, translator_state*);
private:
        std::string name;
};

#endif /* _EXPRESSION_H_INCLUDED */
