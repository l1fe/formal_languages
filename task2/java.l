%option noyywrap

%{
	#include "bisondef.h"
%}

%option yylineno

LETTER			[a-zA-Z]
DIGIT			[0-9]
WORD 			({LETTER}|_)({LETTER}|{DIGIT}|_)*

%%

"public"	{ return PUBLIC; }

"class"		{ return CLASS; }

"static"	{ return STATIC; }

"void"		{ return VOID; }

"main"		{ return MAIN; }

"String[]"	{ return MAIN_ARGS; }

"StaticJavaLib.println"	{ return SLIB_PRINT; }

{DIGIT}+	{
			yylval.value.var_type = vt_int;
			yylval.value.val_int = atoi(yytext);
			return INT;
		}

"true"		{
			yylval.value.var_type = vt_boolean;
			yylval.value.val_boolean = true;
			return BOOLEAN;
		}

"false"		{
			yylval.value.var_type = vt_boolean;
			yylval.value.val_boolean = false;
			return BOOLEAN;
		}
		
"int"		{
			yylval.variable_type = vt_int; 
			return TYPE; 
		}

"boolean"	{ 
			yylval.variable_type = vt_boolean;
			return TYPE; 
		}

{WORD}		{ 
			strcpy(yylval.word, yytext);
			return ID;
		}

[+-/*\()_{}=;,<>.]	{ return *yytext; }

[ \t\n]		;

.		{ fprintf(stderr, "unknown symbol %c", *yytext); }

%%
