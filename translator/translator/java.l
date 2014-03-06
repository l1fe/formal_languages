%{
	#include <stdio.h>
	#include <stdlib.h>

	#include "bisonheader.h"
%}

%option noyywrap

IDENT           [A-Za-z_]*[A-Za-z][A-Za-z0-9_]*
NUMBER          ([1-9][0-9]*)|0

%%

"public"	return PUBLIC;
"static"	return STATIC;
"class"		return CLASS;
"void"		return VOID;
"String[]"	return STRING_ARR;
"for"           return FOR;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"printf"        return PRINTF;
"\"%d\\n\""     return PRINT_FORMAT;
"int"           return INT;
"main"          return MAIN;
"StaticJavaLib.println"	return JAVA_PRINT;


{NUMBER}		{
				yylval.int_val = atoi(yytext); 
				return NUMBER; 
			}

{IDENT}			{
				strcpy(yylval.string_val, yytext); 
				return IDENT; 
			}

[-+*%/{}();,=<>]	return *yytext;

[ \t\n]			/* empty */;

"&&"			return AND;
"||"			return OR;
"!"			return '!';

"=="			return EQUAL;
"!="			return NOT_EQUAL;
"<="			return LESS_OR_EQUAL;
">="			return MORE_OR_EQUAL;

.			{ yyerror("Unknown symbol\n"); }

%%
