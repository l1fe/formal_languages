%{
	//TODO includes
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
	expression_t* exp;
	op_type operation_type;
};

%token <word> ID

// Reserved words
%token PUBLIC
%token CLASS
%token STATIC
%token MAIN_ARGS
%token MAIN
%token VOID
%token SLIB_PRINT

%token <variable_type> TYPE

%token INT
%token BOOLEAN

%type <exp> exp;

%type <stmt> statement;
%type <stmt> assign_statement;
%type <stmt> print_statement;
%type <stmt> local_declaration;

%type <value> literal_exp;

// Priority
%left '='
%left '+' '-'
%left '*' '/'

%%

program:			class_declaration 

class_declaration:		class_header '{' class_body '}'

class_header:			PUBLIC CLASS ID 

class_body:			main_method_declaration '{' method_body '}'

main_method_declaration:	PUBLIC STATIC VOID MAIN '(' MAIN_ARGS ID ')'

method_body:			/* nothing */
				| 
				method_body statement 			{ main_class.add_statement($2); 		}
	
statement:			local_declaration
				| 
				assign_statement
				| 
				print_statement

print_statement:		SLIB_PRINT '(' exp ')' ';' 		{ $$ = new print_statement_t($3);	 	}

local_declaration:		TYPE ID ';' 				{ $$ = new declaration_statement_t($2, $1);	}
		
assign_statement:		ID '=' exp ';' 				{ $$ = new assignment_statement_t($1, $3); 	}

exp:				exp '+' exp	 			{ $$ = new binary_expression_t(op_add, $1, $3); }
				| 
				exp '-' exp 				{ $$ = new binary_expression_t(op_sub, $1, $3); }
				| 
				exp '*' exp		 		{ $$ = new binary_expression_t(op_mul, $1, $3); }
				| 
				exp '/' exp 				{ $$ = new binary_expression_t(op_div, $1, $3); }
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

	main_class.run();
	return 0;	
}

