#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

class Expr {
  public:
    string resultVar;
    virtual void gen(vector<string> &code) = 0;
};

class ExprArith : public Expr {
  public:
    ExprArith(int op_, Expr *e1_, Expr *e2_) : op(op_), e1(e1_), e2(e2_) { }
    void gen(vector<string> &code);
    
  private:
    int op;
    Expr *e1, *e2;
};

class ExprNumeral : public Expr {
  public:
    ExprNumeral(int v) : val(v) {}
    void gen(vector<string> &code);
    int val;
};

class ExprVariable : public Expr {
  public:
    ExprVariable(const char *n) : name(n) { }
    void gen(vector<string> &code);
    
  private:
    string name;
};

class LogExpr {
  public:
    LogExpr() {};
    LogExpr(string op_, Expr *e1_, Expr *e2_) : op(op_), e1(e1_), e2(e2_) { }
    virtual string generate(size_t addrIf, size_t addrElse);
    virtual void gen(vector<string> &code);
    
  private:
    string op;
    Expr *e1, *e2;
};

class LogComplexExpr : public LogExpr {
  public:
    LogComplexExpr(int op_, LogExpr *le1_, LogExpr *le2_) : op(op_), le1(le1_), le2(le2_) { }
    string generate(size_t addrIf, size_t addrElse);
    void gen(vector<string> &code);
    string generateLogExprToBool(LogExpr *e, vector<string> &code);
    
  private:
    int op;
    LogExpr *le1, *le2;
    string resultVar;
    string nullVar;
};

class Stmt { 
  public:
    virtual void gen(vector<string> &code) = 0;
};

class ListStmt : public Stmt {
  public:
    ListStmt() { }
    void add(Stmt *s) { list.push_back(s); }
    void gen(vector<string> &code);
    
  private:
    vector<Stmt*> list;
};

class StmtPrint : public Stmt {
  public:
    StmtPrint(Expr *e_) : e(e_) { }
    void gen(vector<string> &code);
    
  private:
    Expr *e;
};

class StmtIf : public Stmt {
  public:
    StmtIf(LogExpr *e_, Stmt *L1_, Stmt *L2_) : e(e_), L1(L1_), L2(L2_) { }
    void gen(vector<string> &code);
    
  private:
    LogExpr *e;
    Stmt *L1, *L2;
};

class StmtWhile : public Stmt {
  public:
    StmtWhile(LogExpr *e_, Stmt *b): e(e_), body(b) { }
    void gen(vector<string> &code);
    LogExpr *e;
    Stmt *body;
};

class StmtAssign : public Stmt {
  public:
    StmtAssign(const char *n, Expr *e_) : name(n), e(e_) { }
    void gen(vector<string> &code);
    
  private:
    string name;
    Expr *e;
};

class StmtFor : public Stmt {
  public:
    StmtFor(Stmt *p, LogExpr *c, Stmt *i, Stmt *b): prepare(p), condition(c), increment(i), body(b) {}
    void gen(vector<string> &code);
    Stmt *prepare;
    LogExpr *condition;
    Stmt *increment;
    Stmt *body;
};

extern ListStmt *Program;

#include "translator.tab.h"

extern map<string, int> varTable;

void addVariable(char *name, int value);
string strToInt(int number);
string getTmpVariable();

int yylex();
void yyerror(string s);
