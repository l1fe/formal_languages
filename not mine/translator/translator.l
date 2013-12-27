%{
    #include <stdio.h>
    #include <stdlib.h>

    #include "translator.h"
%}

%option noyywrap

IDENT           [A-Za-z_]*[A-Za-z][A-Za-z0-9_]*
NUMBER          ([1-9][0-9]*)|0

%%

"for"           return(FOR);
"while"         return(WHILE);
"if"            return(IF);
"else"          return(ELSE);
"printf"        return(PRINTF);
"\"%d\\n\""     return PRINT_FORMAT;
"int"           return(INT);
"main"          return(MAIN);

{NUMBER}        { yylval.int_val = atoi(yytext); return NUMBER; }
{IDENT}         { strcpy(yylval.string_val, yytext); return IDENT; }

"-"             return('-');
"+"             return('+');
"*"             return('*');
"%"             return('%');
"/"             return('/');

"&&"            return(AND);
"||"            return(OR);
"!"             return('!');

"{"             return('{');
"}"             return('}');
"("             return('(');
")"             return(')');
";"             return(';');
","             return(',');

"="             return('=');

"<"             return '<';
">"             return '>';
"=="            return EQUAL;
"!="            return NOT_EQUAL;
"<="            return LESS_OR_EQUAL;
">="            return MORE_OR_EQUAL;

" "|"\n"|"\t"   /*Nothing can stop the Empire!*/;
.               { printf("Lex error in \"%s\"\n", yytext); exit(1); }

%%
