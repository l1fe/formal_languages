%{
	#include <iostream>
	#include "bisondef.h"

	int yylex(void);
	void yyerror(const char*);

	extern int yylineno;
	extern FILE* yyin;
	extern char* yytext;
	
	FILE* output;

	class_t main_class;
%}

%union {
	char word[256];
	v_type variable_type;
	value_t value;
	statement_t* stmt;
	statement_list_t* stmts;
	expression_t* exp;
	op_type operation_type;
};

%token PUBLIC CLASS STATIC MAIN_ARGS MAIN VOID SLIB_PRINT IF ELSE
%token LE GE NEQ EQ

%token <word> ID
%token <variable_type> TYPE
%token <value> INT BOOLEAN

%type <exp> exp;
%type <stmts> statement_list
%type <stmt> statement;
%type <stmt> assign_statement print_statement local_declaration code_block conditional_statement;
%type <value> literal_exp;

%left '=' NEQ EQ LE GE
%left '+' '-'
%left '*' '/'

%%

program:			class_declaration 

class_declaration:		class_header '{' class_body '}'

class_header:			PUBLIC CLASS ID 

class_body:			main_method_declaration code_block	{ main_class.add_statement($2);			}

main_method_declaration:	PUBLIC STATIC VOID MAIN '(' MAIN_ARGS ID ')'

statement_list:			/* nothing */				{ $$ = new statement_list_t(); 			}
				|
				statement_list statement 		{ $$ = $1;
									  $$->add_statement($2);			}
	
statement:			code_block
				|
				local_declaration
				| 
				assign_statement
				| 
				print_statement
				|
				conditional_statement

code_block:			'{' statement_list '}'			{ $$ = new code_block_statement_t($2);		}

print_statement:		SLIB_PRINT '(' exp ')' ';' 		{ $$ = new print_statement_t($3);	 	}

local_declaration:		TYPE ID ';' 				{ $$ = new declaration_statement_t($2, $1);	}
		
assign_statement:		ID '=' exp ';' 				{ $$ = new assignment_statement_t($1, $3); 	}

conditional_statement:		IF '(' exp ')' statement		{ $$ = new conditional_statement_t($3, $5); 	}
				|
				IF '(' exp ')' statement ELSE statement { $$ = new conditional_statement_t($3, $5, $7);	}

exp:				exp '+' exp	 			{ $$ = new binary_expression_t(op_add, $1, $3); }
				| 
				exp '-' exp 				{ $$ = new binary_expression_t(op_sub, $1, $3); }
				| 
				exp '*' exp		 		{ $$ = new binary_expression_t(op_mul, $1, $3); }
				| 
				exp '/' exp 				{ $$ = new binary_expression_t(op_div, $1, $3); }
				|
				'(' exp ')'				{ $$ = $2;					}
				| 
				literal_exp 				{ $$ = new literal_expression_t($1); 		}
				| 
				ID 					{ $$ = new var_ref_expression_t($1); 		}
				
literal_exp:			BOOLEAN | INT

%%

void yyerror(const char* s) {
	printf("%sError: line number %d:%s ", ANSI_COLOR_RED, yylineno, ANSI_COLOR_RESET);
	printf("%s%s%s\n", ANSI_COLOR_RED, s, ANSI_COLOR_RESET);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "error: correct usage: %s <INPUT_FILE_NAME> <ARG_1> <ARG_2> ... <ARG_N>\n", argv[0]);
		exit(1);
	}

	yyin = fopen(argv[1], "r");
	if (yyin == NULL) {
		fprintf(stderr, "error:	can't open file %s for reading\n", argv[1]);
		exit(1);
	}
	
	/*
	for (int i = 2; i < argc; ++i) {
		std::string arg(argv[i]);
		main_class.main_method.add_argument(arg);
	}
	*/
	
	yyparse();

	std::cout << "Parse OK" << std::endl;
	main_class.run();
	return 0;	
}

