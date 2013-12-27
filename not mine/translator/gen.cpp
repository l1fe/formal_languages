#include <iostream>
#include <cstdlib>
#include <sstream>

#include "translator.h"

using namespace std;

void ExprNumeral::gen(vector<string> &code) {
    resultVar = getTmpVariable();
    code.push_back("let " + resultVar + " " + strToInt(val));
}

void ExprArith::gen(vector<string> &code) {
    e1->gen(code); 
    if (e2 != NULL) e2->gen(code);

    resultVar = getTmpVariable();

    switch(op) {
        case '+':
            code.push_back("add " + e1->resultVar + " " + e2->resultVar + " " + resultVar);
            break;
            
        case '-': 
            code.push_back("sub " + e1->resultVar + " " + e2->resultVar + " " +  resultVar);
            break;
            
        case '*': 
            code.push_back("mul " + e1->resultVar + " " + e2->resultVar + " " + resultVar);
            break;
            
        case '/': 
            code.push_back("div " + e1->resultVar + " " + e2->resultVar + " " + resultVar);
            break;
            
        case '%': {
            string tmpvar = getTmpVariable();
            code.push_back("div " + e1->resultVar + " " + e2->resultVar + " " + tmpvar);
            code.push_back("mul " + tmpvar + " " + e2->resultVar + " " + tmpvar);
            code.push_back("sub " + e1->resultVar + " " + tmpvar + " " + resultVar);
            break;
        }
        
        case UMINUS: {
            string tmpvar = getTmpVariable();
            code.push_back("let " + tmpvar + " 0");
            code.push_back("sub " + tmpvar + " " + e1->resultVar + " " + resultVar);
            break;
        }
    }
}

void ExprVariable::gen(vector<string> &code) {
    if (varTable.find(name) == varTable.end()) {
        cerr << "Runtime error: " << name << " not defined\n";
        exit(1);        
    }
    
    resultVar = name;
}

void LogExpr::gen(vector<string> &code) {
    e1->gen(code); 
    e2->gen(code);
}

string LogExpr::generate(size_t addrIf, size_t addrElse) {
    string result = "cmp " + e1->resultVar + " " + e2->resultVar + " ";
    //++addrIf;
    //++addrElse;
    
    if (op == "<") {result += strToInt(addrIf) + " " + strToInt(addrElse) + " " + strToInt(addrElse); return result; }
    if (op == ">") {result += strToInt(addrElse) + " " + strToInt(addrElse) + " " + strToInt(addrIf); return result; }
    if (op == "==") {result += strToInt(addrElse) + " " + strToInt(addrIf) + " " + strToInt(addrElse); return result; }
    
    if (op == "!=") {result += strToInt(addrIf) + " " + strToInt(addrElse) + " " + strToInt(addrIf); return result; }
    if (op == ">=") {result += strToInt(addrElse) + " " + strToInt(addrIf) + " " + strToInt(addrIf); return result; }
    if (op == "<=") {result += strToInt(addrIf) + " " + strToInt(addrIf) + " " + strToInt(addrElse); return result; }
    
    return result;
}

void ListStmt::gen(vector<string> &code) {
    for (vector<Stmt*>::iterator i = list.begin(); i < list.end(); i++) (*i)->gen(code);
}

void StmtPrint::gen(vector<string> &code) {
    e->gen(code);
    code.push_back("out " + e->resultVar);
}

void StmtIf::gen(vector<string> &code) {
    e->gen(code);

    size_t IfPos = code.size();
    size_t JumpForwardPos;

    code.push_back("");

    L1->gen(code);
    size_t ElsePos = code.size();

    if (L2 != NULL) {
        ++ElsePos;
        JumpForwardPos = code.size();
        code.push_back("");
        L2->gen(code);
    }

    code[IfPos] = e->generate(IfPos + 1, ElsePos);
    
    if (L2 != NULL) code[JumpForwardPos] = "jmp " + strToInt(code.size());

}

void StmtAssign::gen(vector<string> &code) {
    if (varTable.find(name) == varTable.end()) {
        cerr << "Runtime error: " << name << " undefined\n";
        exit(1);        
    }

    ExprNumeral *en = dynamic_cast<ExprNumeral *>(e);
    if (en == NULL) {
        e->gen(code);
        code.push_back("mov " + e->resultVar + " " + name);
    } else code.push_back("let " + name + " " + strToInt(en->val));
}

void StmtWhile::gen(vector<string> &code) {
    size_t StartPos = code.size();
    e->gen(code);
    size_t CondPos = code.size();
    code.push_back("");
    body->gen(code);
    code.push_back("jmp " + strToInt(StartPos));
    size_t ExitPos = code.size();
    code[CondPos] = e->generate(CondPos + 1, ExitPos);
}

void StmtFor::gen(vector<string> &code) {
    prepare->gen(code);
    size_t StartPos = code.size();
    condition->gen(code);
    size_t CondPos = code.size();
    code.push_back("");
    body->gen(code);
    increment->gen(code);
    code.push_back("jmp " + strToInt(StartPos));
    size_t ExitPos = code.size();
    code[CondPos] = condition->generate(CondPos + 1, ExitPos);
}

string LogComplexExpr::generate(size_t addrIf, size_t addrElse) {
    //++addrIf;
    //++addrElse;
    return "cmp " + resultVar + " " + nullVar + " " + strToInt(addrElse) + " " + strToInt(addrElse) + " " + strToInt(addrIf);
}

void LogComplexExpr::gen(vector<string> &code) {
    nullVar = getTmpVariable();
    code.push_back("let " + nullVar + " 0");
    string e1Name = generateLogExprToBool(le1, code);
    string e2Name;
    if (le2 != NULL) e2Name = generateLogExprToBool(le2, code);
        
    resultVar = getTmpVariable();

    switch (op) {
        case AND:
            code.push_back("mul " + e1Name + " " + e2Name + " " + resultVar);
            break;
            
        case OR: { 
            code.push_back("add " + e1Name + " " + e2Name + " " + resultVar);
            size_t actualPos = code.size();
            code.push_back("cmp " + resultVar + " " + nullVar + " " +  strToInt(actualPos + 3) + " " + strToInt(actualPos + 3) + " " + strToInt(actualPos + 2));
            code.push_back("let " + resultVar + " 1");
            break;
        }
        
        case '!':
            code.push_back("let " + resultVar + " 1");
            code.push_back("sub " + resultVar + " " + e1Name + " " + resultVar);
        break;
    }
}

string LogComplexExpr::generateLogExprToBool(LogExpr *e, vector<string> &code) {
    e->gen(code);

    string result = getTmpVariable();
    code.push_back("let " + result + " 0");

    size_t IfPos = code.size();
    code.push_back(e->generate(IfPos + 1, IfPos + 2));
    code.push_back("let " + result + " 1");

    return result;
}

ListStmt *Program;
size_t TEMP_VARS = 0;

string strToInt(int number) {
    stringstream strStr;
    strStr<<number;
    return strStr.str();
}

string getTmpVariable() {
    string newName = "tmp" + strToInt(TEMP_VARS++);
    while (varTable.find(newName) != varTable.end()) {
        newName = "tmp" + strToInt(TEMP_VARS++);
    }
    
    return newName;
}
