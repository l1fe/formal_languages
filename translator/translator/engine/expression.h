#ifndef _EXPRESSION_H_INCLUDED_
#define _EXPRESSION_H_INCLUDED_

#include <string>
#include <vector>
#include <map>

class expression_t {
public:
        virtual void generate_code(std::vector<std::string>& code) = 0;

        std::string result_variable;
};

class arithmetic_expression_t : public expression_t {
public:
        arithmetic_expression_t(int op_, expression_t* e1_, expression_t* e2_) : op(op_), e1(e1_), e2(e2_) { }

        virtual void generate_code(std::vector<std::string>& code);
private:
        int op;
        expression_t* e1;
        expression_t* e2;
};

class numeral_expression_t : public expression_t {
public:
        numeral_expression_t(int v) : val(v) {}

        virtual void generate_code(std::vector<std::string>& code);

        int val;
};

class variable_expression_t : public expression_t {
public:
        variable_expression_t(const char* n) : name(n) { }

        virtual void generate_code(std::vector<std::string>& code);
private:
        std::string name;
};

class logic_expression_t {
public:
        logic_expression_t() { }
        logic_expression_t(std::string op_, expression_t* e1_, expression_t* e2_) : op(op_), e1(e1_), e2(e2_) { }

        virtual std::string generate(size_t, size_t);
        virtual void generate_code(std::vector<std::string>& code);
private:
        std::string op;
        expression_t* e1;
        expression_t* e2;
};

class logic_complex_expression_t : public logic_expression_t {
public:
        logic_complex_expression_t(int op_, logic_expression_t* le1_, logic_expression_t* le2_) : op(op_), le1(le1_), le2(le2_) { }

        std::string generate(size_t addrIf, size_t addrElse);
        virtual void generate_code(std::vector<std::string>& code);
        std::string generate_logic_expression_t_to_bool(logic_expression_t* e, std::vector<std::string>& code);
private:
        int op;
        logic_expression_t* le1;
        logic_expression_t* le2;
        std::string result_variable;
        std::string null_variable;
};

#endif /* _EXPRESSION_H_INCLUDED_ */
