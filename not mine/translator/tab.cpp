#include <cstring>
#include <cstdlib>
#include <iostream>

#include "translator.h"

using namespace std;

map<string, int> varTable;

void addVariable(char *name, int value) {
    if (varTable.find(name) != varTable.end()) {
        cout<<"Redeclaration of "<<name<< endl;
        exit(1);
    }

    varTable.insert(make_pair(name, value));
}
