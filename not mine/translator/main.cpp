#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "translator.h"

using namespace std;

void yyparse();

extern FILE* yyin;

int main(int argc, char **argv) {
    FILE *in;
    ofstream output("out.txt", ofstream::out);
    if (!output.is_open()) { cout<<"Can not write to \'out.txt\'!\n"; exit(1); }
     
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " program\n";
        exit(1);
    }
    
    if (!(in = fopen(argv[1], "r"))) {
        cerr << "Error: cannot open file\n";
        exit(1);
    }
    
    yyin = in;
    yyparse();

    vector<string> code;
    for (map<string, int>::iterator i = varTable.begin(); i != varTable.end(); ++i) {
        code.push_back("let " + i->first + " " + strToInt(i->second));
    }
    
    Program->gen(code);

    for (vector<string>::iterator i = code.begin(); i != code.end(); ++i) {
        output<<*i<<endl;
    }
    
    output.close();
    return 0;
}

void yyerror(string s) {
    cerr<<"Parser error: "<<s<<"\n";
    exit(1);
}
