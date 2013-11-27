%{
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "command_types.h"
#include "./static_hash_table/static_hash_table.h"
#include "./utils/utils.h"
#include "./state/state.h"

commands_state* state;

void lets_exit(int);

%}

NUMBER		[0-9]+
VARIABLE	[a-zA-Z_][a-zA-Z0-9_]*
SPACE		[ \t]+

%%

let			{ set_current_command(state, C_LET, 2); }
mov			{ set_current_command(state, C_MOV, 2); }
add			{ set_current_command(state, C_ADD, 3); }
sub			{ set_current_command(state, C_SUB, 3); }
mul			{ set_current_command(state, C_MUL, 3); }
div			{ set_current_command(state, C_DIV, 3); }
jmp			{ set_current_command(state, C_JMP, 1); }
cmp			{ set_current_command(state, C_CMP, 5); }
out			{ set_current_command(state, C_OUT, 1); }
table			{ static_hash_table_print(state->vars_h_table); }

{VARIABLE} 		{
				bool add_success = add_command_argument(state, 0, strdup(yytext), A_NAME); 
				if ( !add_success ) {
					fprintf(stderr, "[error]: syntax error: invalid command arguments at line %d\n", state->current_line);
					yyterminate();
		  		}
		
				int command_is_correct_indicator;
				bool command_is_ready = check_current_command_ready(state, &command_is_correct_indicator);

				if (command_is_correct_indicator == -1) {
					fprintf(stderr, "[error]: syntax error: invalid command arguments at line %d\n", state->current_line);
					yyterminate();
		  		}

				if ( command_is_ready ) {
					bool execute_success = execute_current_command(state);
					if ( !execute_success ) {
						fprintf(stderr, "[error]: syntax error: can't execute command at line %d\n", state->current_line);
						yyterminate();
					}
				}
			}

{NUMBER}		{
				bool add_success = add_command_argument(state, atoi(yytext), NULL, A_INT); 
				if ( !add_success ) {
					fprintf(stderr, "[error]: syntax error: invalid command arguments at line %d\n", state->current_line);
					yyterminate();
		  		}
		
				int command_is_correct_indicator;
				bool command_is_ready = check_current_command_ready(state, &command_is_correct_indicator);

				if (command_is_correct_indicator == -1) {
					fprintf(stderr, "[error]: syntax error: invalid command arguments at line %d\n", state->current_line);
					yyterminate();
		  		}

				if ( command_is_ready ) {
					bool execute_success = execute_current_command(state);
					if ( !execute_success ) {
						fprintf(stderr, "[error]: syntax error: can't execute command at line %d\n", state->current_line);
						yyterminate();
					}
				}
			}

{SPACE}			{ 
				/* skip */ 
			}

\n			{ 
				if (state->current_command != C_EMPTY) {
					fprintf(stderr, "[error]: syntax error, invalid command arguments at line %d\n", state->current_line);
					yyterminate();
		  		}

				state->current_line += 1;
			}

.			{ 
				fprintf(stderr, "[error]: unresolved symbol \'%s\' at line %d\n", yytext, state->current_line);
				yyterminate();
			}

<<EOF>>			{ 
				if (state->current_command != C_EMPTY) {
					fprintf(stderr, "[error]: unexpected EOF\n");
				}
		  
				yyterminate();
			}
%%

void lets_exit(int status) {
	destroy_commands_state(state);
	yylex_destroy();

	exit(status);

}
int main() {
	state = create_commands_state();

	yylex();

	lets_exit(0);
}
