%{
	#include <stdio.h>
	#include <stdlib.h>

	#include "bisonheader.h"
%}

%union {
	int int_val;
	char string_val[256];

	expression_t* expr;
	logic_expression_t* logic;
	
	statement_t* statement;
	statement_list_t* statement_list;
};

%left '+' '-'
%left '%' '*' '/'
%left '<' '>' EQUAL NOT_EQUAL LESS_OR_EQUAL MORE_OR_EQUAL
%left AND OR

%nonassoc UMINUS '!'

%token <int_val> NUMBER
%token <string_val> IDENT
%token PUBLIC STATIC VOID MAIN STRING_ARR WHILE FOR IF ELSE INT CLASS JAVA_PRINT
%token EQUAL NOT_EQUAL LESS_OR_EQUAL MORE_OR_EQUAL AND OR

%right ')' ELSE

%type <expr> expr
%type <logic> logic
%type <statement_list> statement_list
%type <statement_list> statement_body
%type <statement> assign
%type <statement> statement

%start main

%%

main: PUBLIC CLASS IDENT '{' PUBLIC STATIC VOID MAIN '(' STRING_ARR IDENT ')' statement_body '}' 	{
														Program = $13; 
													}
    
separator: ';'						{
								/* empty */
							}

statement_body:
	'{' '}'                                   	{ 
								$$ = new statement_list_t(); 
							}
	|
	'{' declaration_list '}'                  	{ 
								$$ = new statement_list_t(); 
							}
	|
	'{' declaration_list statement_list '}'   	{ 
								$$ = $3; 
							}
	|
	'{' statement_list '}'                    	{ 
								$$ = $2; 
							}

declaration_list:
	declaration					{ /* empty */ }
    	|
	declaration_list declaration			{ /* empty */ }

declaration: 
	INT IDENT separator                       	{ 
								add_new_var($2, 0); 
							}
	| 
	INT IDENT '=' NUMBER separator            	{ 
								add_new_var($2, $4); 
							}
statement_list:
	statement_list statement                  	{ 
								$1->add($2); 
								$$ = $1; 
							}
    	| 
	statement                                 	{ 
								$$ = new statement_list_t; 
								$$->add($1); 
							}
statement: 
	JAVA_PRINT '(' expr ')' separator		{
								$$ = new print_statement_t($3); 
							}
	| 
	assign separator                                { 
								$$ = $1; 
							}
	|
	IF '(' logic ')' statement                    	{ 
								$$ = new if_statement_t($3, $5, NULL); 
							}
    	| 
	IF '(' logic ')' statement ELSE statement	{ 
								$$ = new if_statement_t($3, $5, $7); 
							}
	| 
	'{' statement_list '}'                          { 
								$$ = $2; 
							}
    	| 
	WHILE '(' logic ')' statement			{ 
								$$ = new while_statement_t($3, $5); 
							} 
	| 
	FOR '(' assign separator logic separator assign ')' statement 	{ 
										$$ = new for_statement_t($3, $5, $7, $9); 
									}

assign:
	IDENT '=' expr 					{ 
								$$ = new assign_statement_t($1, $3); 
							}

logic: 
	expr '<' expr                 			{ 
								$$ = new logic_expression_t("<", $1, $3); 
							}
    	| 
	expr '>' expr                 			{ 
								$$ = new logic_expression_t(">", $1, $3); 
							}
	|
	expr LESS_OR_EQUAL  expr      			{ 
								$$ = new logic_expression_t("<=", $1, $3); 
							}
	| 
	expr MORE_OR_EQUAL expr       			{ 
								$$ = new logic_expression_t(">=", $1, $3); 
							}
    	| 
	expr NOT_EQUAL expr           			{ 
								$$ = new logic_expression_t("!=", $1, $3); 
							}
	| 
	expr EQUAL expr               			{ 
								$$ = new logic_expression_t("==", $1, $3); 
							}
	| 
	logic OR logic                			{ 
								$$ = new logic_complex_expression_t(OR, $1, $3); 
							}
	|
	logic AND logic               			{ 
								$$ = new logic_complex_expression_t(AND, $1, $3); 
							}
	| 
	'!' logic                     			{ 
								$$ = new logic_complex_expression_t('!', $2, NULL); 
							}
	| 
	'(' logic ')'                 			{ 
								$$ = $2; 
							} 
expr:
	IDENT                         			{ 
								$$ = new variable_expression_t($1); 
							}
	|
	NUMBER                        			{ 
								$$ = new numeral_expression_t($1); 
							}
	| 
	'(' expr ')'                  			{ 
								$$ = $2; 
							}
	|
	'-' expr %prec UMINUS         			{ 
								$$ = new arithmetic_expression_t(UMINUS, $2, NULL); 
							}
	|
	expr '+' expr                 			{ 
								$$ = new arithmetic_expression_t('+', $1, $3); 
							}
	|
	expr '-' expr                 			{ 
								$$ = new arithmetic_expression_t('-', $1, $3); 
							}
	| 
	expr '*' expr                 			{ 
								$$ = new arithmetic_expression_t('*', $1, $3); 
							}
	| 
	expr '/' expr                 			{
								$$ = new arithmetic_expression_t('/', $1, $3); 
							}    
    	| 
	expr '%' expr                 			{ 
								$$ = new arithmetic_expression_t('%', $1, $3); 
							}
%%
