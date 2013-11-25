%{
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "command_types.h"
#include "static_hash_table.h"
#include "utils.h"
#include "state.h"

commands_state* state;
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

{VARIABLE} 	{
				bool add_success = add_command_argument(state, strdup(yytext), A_NAME); 
				if ( !add_success ) {
					fprintf(stderr, "[error]: syntax error: invalid command arguments at line %d\n", state->current_line);
					exit(1);
		  		}
		
				int command_is_correct_indicator;
				bool command_is_ready = check_current_command_ready(state, &command_is_correct_indicator);

				if (command_is_correct_indicator == -1) {
					fprintf(stderr, "[error]: syntax error: invalid command arguments at line %d\n", state->current_line);
					exit(1);
		  		}

				if ( command_is_ready ) {
					bool execute_success = execute_current_command(state);
					if ( !execute_success ) {
						fprintf(stderr, "[error]: syntax error: can't execute command at line %d\n", state->current_line);
						exit(1);
					}
				}
			}

{NUMBER}	{
				bool add_success = add_command_argument(state, strdup(yytext), A_INT); 
				if ( !add_success ) {
					fprintf(stderr, "[error]: syntax error: invalid command arguments at line %d\n", state->current_line);
					exit(1);
		  		}
		
				int command_is_correct_indicator;
				bool command_is_ready = check_current_command_ready(state, &command_is_correct_indicator);

				if (command_is_correct_indicator == -1) {
					fprintf(stderr, "[error]: syntax error: invalid command arguments at line %d\n", state->current_line);
					exit(1);
		  		}

				if ( command_is_ready ) {
					bool execute_success = execute_current_command(state);
					if ( !execute_success ) {
						fprintf(stderr, "[error]: syntax error: can't execute command at line %d\n", state->current_line);
						exit(1);
					}
				}
			}

{SPACE}		{ 
				/* skip */ 
			}

\n			{ 
				if (state->current_command != C_EMPTY) {
					fprintf(stderr, "[error]: syntax error, invalid command arguments at line %d\n", state->current_line);
					exit(1);
		  		}

				state->current_line += 1;
			}

.			{ 
				fprintf(stderr, "[error]: unresolved symbol \'%s\' at line %d\n", yytext, state->current_line);
				exit(1); 
			}

<<EOF>>		{ 
				if (state->current_command != C_EMPTY) {
					abort_prg("[error]: unexpected end of file");
				}
		  
				yyterminate();
			}
%%

int main() {
	state = create_commands_state();

	yylex();

	destroy_commands_state(state);
	yylex_destroy();

	return 0;
}
