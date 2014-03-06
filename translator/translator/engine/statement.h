#ifndef _STATEMENT_H_INCLUDED_
#define _STATEMENT_H_INCLUDED_

#include <iostream>
#include <vector>
#include <string>

#include "expression.h"

class statement_t { 
public:
        virtual void generate(std::vector<std::string>& code) = 0;
};

class statement_list_t : public statement_t {
public:
        statement_list_t() { }

        void add(statement_t* s) {
                list.push_back(s); 
        }
        virtual void generate(std::vector<std::string>& code);

private:
        std::vector<statement_t*> list;
};

class print_statement_t : public statement_t {
public:
        print_statement_t(expression_t* e_) : e(e_) { }
        
        virtual void generate(std::vector<std::string>& code);
private:
        expression_t* e;
};

class if_statement_t : public statement_t {
public:
        if_statement_t(logic_expression_t* e_, statement_t* L1_, statement_t* L2_) : e(e_), L1(L1_), L2(L2_) { }

        virtual void generate(std::vector<std::string>& code);
private:
        logic_expression_t* e;
        statement_t* L1;
        statement_t* L2;
};

class while_statement_t : public statement_t {
public:
        while_statement_t(logic_expression_t* e_, statement_t* b): e(e_), body(b) { }

        virtual void generate(std::vector<std::string>& code);

        logic_expression_t* e;
        statement_t* body;
};

class assign_statement_t : public statement_t {
public:
        assign_statement_t(const char* n, expression_t* e_) : name(n), e(e_) { }

        virtual void generate(std::vector<std::string>& code);
private:
        std::string name;
        expression_t* e;
};

class for_statement_t : public statement_t {
public:
        for_statement_t(statement_t* p, logic_expression_t* c, statement_t* i, statement_t* b): prepare(p), condition(c), increment(i), body(b) {}

        virtual void generate(std::vector<std::string>& code);

        statement_t* prepare;
        logic_expression_t* condition;
        statement_t* increment;
        statement_t* body;
};

#endif /* _STATEMENT_H_INCLUDED_ */
