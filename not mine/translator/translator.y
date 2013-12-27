%{
    #include <stdio.h>
    #include <stdlib.h>
    
    #include "translator.h"
%}

%union {
    Expr *expr;
    LogExpr *logic;
    Stmt *statement;
    ListStmt *statement_list;

    int int_val;
    char string_val[256];
};

%left '+' '-'
%left '%' '*' '/'
%left '<' '>' EQUAL NOT_EQUAL LESS_OR_EQUAL MORE_OR_EQUAL
%left AND OR

%nonassoc UMINUS '!'

%token <int_val> NUMBER
%token <string_val> IDENT
%token <string_val> PRINT_FORMAT
%token WHILE FOR IF ELSE PRINTF INT MAIN
%token EQUAL NOT_EQUAL LESS_OR_EQUAL MORE_OR_EQUAL AND OR

%type <expr> expr
%type <logic> logic
%type <statement_list> statement_list
%type <statement_list> statement_body
%type <statement> assign
%type <statement> statement

%start main

%%

main: INT MAIN '(' ')' statement_body { Program = $5; } ;
    
separator: ';' ;

statement_body
    : '{' '}'                                   { $$ = new ListStmt(); }
    | '{' declaration_list '}'                  { $$ = new ListStmt(); }
    | '{' declaration_list statement_list '}'   { $$ = $3; }
    | '{' statement_list '}'                    { $$ = $2; }
    ;

declaration_list
    : declaration
    | declaration_list declaration
    ;

declaration
    : INT IDENT separator                       { addVariable($2, 0); }
    | INT IDENT '=' NUMBER separator            { addVariable($2, $4); }
    ;

statement_list
    : statement_list statement                  { $1->add($2); $$ = $1; }
    | statement                                 { $$ = new ListStmt; $$->add($1); }
    ;

statement
    : PRINTF '(' PRINT_FORMAT ',' expr ')' separator                { $$ = new StmtPrint($5); } 
    | assign separator                                              { $$ = $1; }
    | IF '(' logic ')' statement                                    { $$ = new StmtIf($3, $5, NULL); }
    | IF '(' logic ')' statement ELSE statement                     { $$ = new StmtIf($3, $5, $7); }
    | '{' statement_list '}'                                        { $$ = $2; }
    | WHILE '(' logic ')' statement                                 { $$ = new StmtWhile($3, $5); } 
    | FOR '(' assign separator logic separator assign ')' statement { $$ = new StmtFor($3, $5, $7, $9); }
    ;

assign
    : IDENT '=' expr { $$ = new StmtAssign($1, $3); }
    ;

logic
    : expr '<' expr                 { $$ = new LogExpr("<", $1, $3); }
    | expr '>' expr                 { $$ = new LogExpr(">", $1, $3); }
    | expr LESS_OR_EQUAL  expr      { $$ = new LogExpr("<=", $1, $3); }
    | expr MORE_OR_EQUAL expr       { $$ = new LogExpr(">=", $1, $3); }
    | expr NOT_EQUAL expr           { $$ = new LogExpr("!=", $1, $3); }
    | expr EQUAL expr               { $$ = new LogExpr("==", $1, $3); }
    | logic OR logic                { $$ = new LogComplexExpr(OR, $1, $3); }
    | logic AND logic               { $$ = new LogComplexExpr(AND, $1, $3); }
    | '!' logic                     { $$ = new LogComplexExpr('!', $2, NULL); }
    | '(' logic ')'                 { $$ = $2; } 
    ;

expr
    : IDENT                         { $$ = new ExprVariable($1); }
    | NUMBER                        { $$ = new ExprNumeral($1); }
    | '(' expr ')'                  { $$ = $2; }
    | '-' expr %prec UMINUS         { $$ = new ExprArith(UMINUS, $2, NULL); }
    | expr '+' expr                 { $$ = new ExprArith('+', $1, $3); }
    | expr '-' expr                 { $$ = new ExprArith('-', $1, $3); }
    | expr '*' expr                 { $$ = new ExprArith('*', $1, $3); }
    | expr '/' expr                 { $$ = new ExprArith('/', $1, $3); }    
    | expr '%' expr                 { $$ = new ExprArith('%', $1, $3); } 
    ;

%%
