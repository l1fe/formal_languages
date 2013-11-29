#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "../command_types.h"
#include "../static_hash_table/static_hash_table.h"
#include "../dynamic_array/dynamic_array.h"
#include "../state/state.h"
#include "../utils/utils.h"

#define ALLOW_UNDECLARED false

commands_state* create_commands_state(FILE* input_file, FILE* output_file) {
	commands_state* state = (commands_state*)malloc(sizeof(commands_state));
	if (state == NULL) {
		abort_prg("create_commands_state: malloc error");
	}
	
	state->default_init_value = 0;
	state->current_command = C_EMPTY;
	state->current_command_args_num = 0;
	state->vars_h_table = static_hash_table_create();
	state->current_arg_index = 0;

	int i;
	for (i = 0; i < MAX_ARGS_NUM; ++i) {
		state->args[i].argument_type = A_EMPTY;
		state->args[i].var_a_name = NULL;
		state->args[i].var_a_int = state->default_init_value;
	}

	if (input_file == NULL) {
		state->input = stdin;
	} else {
		state->input = input_file;
	}

	if (output_file == NULL) {
		state->output = stdout;
	} else {
		state->output = output_file;
	}
	
	state->current_line = 0;

	state->input_lines_info = dynamic_array_create(100);
	
	bool get_input_lines_info_success = get_input_lines_info(state);
	
	if ( !get_input_lines_info_success ) {
		fprintf(stderr, "create_commands_state:: error, can't get input lines info\n");
	}

	return state;
}

command_t get_current_command(commands_state* state) {
	return state->current_command;
}


bool set_current_command(commands_state* state, command_t command, int command_args_num) {
	if (state == NULL || command_args_num < 0) {
		return false;
	}

	state->current_command = command;
	state->current_command_args_num = command_args_num;

	return true;
}

int get_var_value_by_name(commands_state* state, char* var_name, int* success_indicator) {
	if (state == NULL || state->vars_h_table == NULL || var_name == NULL) {
		*success_indicator = -1;
		return -1;
	}
	
	int success;
	int ht_get_result = static_hash_table_get(state->vars_h_table, var_name, &success);
	if (success < 0) {
		*success_indicator = -1;
		return -1;
	}

	*success_indicator = 0;

	return ht_get_result;
}

bool add_var(commands_state* state, char* var_name, int var_value) {
	if (state == NULL || state->vars_h_table == NULL || var_name == NULL) {
		return false;
	}

	state->vars_h_table = static_hash_table_put(state->vars_h_table, var_name, var_value);

	if (state->vars_h_table == NULL) {
		abort_prg("add_var: hash_table_put error");
	}

	return true;
}

bool check_current_command_ready(commands_state* state, int* ind) {
	if (state == NULL || state->vars_h_table == NULL) {
		*ind = -1;
		return false;
	}
	
	if (state->current_command == C_EMPTY) {
		*ind = -1;
		return false;
	}
	
	int args_not_empty_count = 0;
	int i;
	for (i = 0; i < MAX_ARGS_NUM; ++i) {
		if (state->args[i].argument_type == A_EMPTY) {
			break;
		}

		if (state->args[i].argument_type == A_NAME && state->args[i].var_a_name == NULL) {
			*ind = -1;
			return false;
		}

		args_not_empty_count += 1;
	}

	if (args_not_empty_count < state->current_command_args_num) {
		// command is not ready to execute but still correct
		*ind = 0;
		return false;
	}

	switch (state->current_command) {
		case C_LET:
			if (state->args[0].argument_type == A_NAME
			    && state->args[1].argument_type == A_INT
			) {
				*ind = 0;
				return true;
			} else {
				*ind = -1;
				return false;
			}
		case C_MOV:
			if (state->args[0].argument_type == A_NAME
			    && state->args[1].argument_type == A_NAME
			) {
				*ind = 0;
				return true;
			} else {
				*ind = -1;
				return false;
			}
		case C_ADD:
			if (state->args[0].argument_type == A_NAME
			    && state->args[1].argument_type == A_NAME
			    && state->args[2].argument_type == A_NAME
			) {
				*ind = 0;
				return true;
			} else {
				*ind = -1;
				return false;
			}
		case C_SUB:
			if (state->args[0].argument_type == A_NAME
			    && state->args[1].argument_type == A_NAME
			    && state->args[2].argument_type == A_NAME
			) {
				*ind = 0;
				return true;
			} else {
				*ind = -1;
				return false;
			}
		case C_MUL:
			if (state->args[0].argument_type == A_NAME
			    && state->args[1].argument_type == A_NAME
			    && state->args[2].argument_type == A_NAME
			) {
				*ind = 0;
				return true;
			} else {
				*ind = -1;
				return false;
			}
		case C_DIV:
			if (state->args[0].argument_type == A_NAME
			    && state->args[1].argument_type == A_NAME
			    && state->args[2].argument_type == A_NAME
			) {
				*ind = 0;
				return true;
			} else {
				*ind = -1;
				return false;
			}
		case C_JMP:
			if (state->args[0].argument_type == A_INT) {
				*ind = 0;
				return true;
			} else {
				*ind = -1;
				return false;
			}
		case C_CMP:
			if (state->args[0].argument_type == A_NAME
			    && state->args[1].argument_type == A_NAME
			    && state->args[2].argument_type == A_INT
			    && state->args[3].argument_type == A_INT
			    && state->args[4].argument_type == A_INT
			) {
				*ind = 0;
				return true;
			} else {
				*ind = -1;
				return false;
			}
		case C_OUT:
			if (state->args[0].argument_type == A_NAME) {
				*ind = 0;
				return true;
			} else {
				*ind = -1;
				return false;
			}
		default:
			// unknown command
			*ind = -1;
			return false;
	}

	// unkown
	*ind = -1;
	return false;
}

bool add_command_argument(commands_state* state, int var_a_int, char* var_a_name, argument_t argument_type) {
	if (state == NULL || state->vars_h_table == NULL) {
		return false;
	}
	
	if (state->current_arg_index >= MAX_ARGS_NUM) {
		return false;
	}

	if (argument_type == A_NAME && var_a_name == NULL) {
		return false;
	}

	argument_struct argument_to_add;
	argument_to_add.argument_type = argument_type;
	argument_to_add.var_a_int = var_a_int;
	argument_to_add.var_a_name = var_a_name;

	state->args[state->current_arg_index] = argument_to_add;
	state->current_arg_index += 1;

	return true;
}

void clear_commands_state(commands_state* state) {
	if (state == NULL) {
		return;
	}

	state->current_command = C_EMPTY;
	state->current_command_args_num = 0;
	
	unsigned short i;
	for (i = 0; i < MAX_ARGS_NUM; ++i) {
		free(state->args[i].var_a_name);
		state->args[i].var_a_name = NULL;
		state->args[i].var_a_int = state->default_init_value;
		state->args[i].argument_type = A_EMPTY;
	}

	state->current_arg_index = 0;
}

//unsafe
void destroy_commands_state(commands_state* state) {
	if (state == NULL) {
		return;
	}

	static_hash_table_drop_table(state->vars_h_table);
	free(state);
}

//unsafe
bool execute_C_LET(commands_state* state) {
	char* a_name = state->args[0].var_a_name;	
	int a_value = state->args[1].var_a_int;

	return add_var(state, a_name, a_value);
}

//unsafe
bool execute_C_MOV(commands_state* state) {	
	char* a_name = state->args[0].var_a_name;
	char* b_name = state->args[1].var_a_name;

	int success;
	int a_value = get_var_value_by_name(state, a_name, &success);

	if (success < 0) {
		if (ALLOW_UNDECLARED) {
			bool add_var_a_success;
			add_var_a_success = add_var(state, a_name, state->default_init_value);

			if ( !add_var_a_success ) {
				return false;
			}

			a_value = state->default_init_value;
		} else {
			return false;
		}
	}

	get_var_value_by_name(state, b_name, &success);
	
	if (success < 0 && !ALLOW_UNDECLARED) {
		return false;
	}

	return add_var(state, b_name, a_value);
}

//unsafe
bool execute_C_OUT(commands_state* state) {
	char* a_name = state->args[0].var_a_name;

	int success;
	int a_value = get_var_value_by_name(state, a_name, &success);


	if (success < 0) {
		if (ALLOW_UNDECLARED) {
			bool add_var_success = true;
			add_var_success = add_var(state, a_name, state->default_init_value);		
			if ( !add_var_success ) {
				return false;
			}
		
			a_value = state->default_init_value;
		} else {
			return false;
		}
	}

	fprintf(state->output, "%d\n", a_value);

	return true;
}

//unsafe
bool execute_C_ADD(commands_state* state) {
	char* a_name = state->args[0].var_a_name;
	char* b_name = state->args[1].var_a_name;
	char* c_name = state->args[2].var_a_name;

	int success;
	int a_value = get_var_value_by_name(state, a_name, &success);
	if (success < 0) {
		if (ALLOW_UNDECLARED) {
			bool add_var_succ = add_var(state, a_name, state->default_init_value);
			if ( !add_var_succ ) {
				return false;
			}
		
			a_value = state->default_init_value;
		} else {
			return false;
		}
	}

	int b_value = get_var_value_by_name(state, b_name, &success);
	if (success < 0) {
		if (ALLOW_UNDECLARED) {
			printf("add: var not found\n");
			bool add_var_succ = add_var(state, b_name, state->default_init_value);
			if ( !add_var_succ ) {
				return false;
			}

			b_value = state->default_init_value;
		} else {
			return false;
		}
	}

	get_var_value_by_name(state, c_name, &success);

	if (success < 0 && !ALLOW_UNDECLARED) {
		return false;
	}

	if (INT_MAX - a_value < b_value || INT_MAX - b_value < a_value) {
		return false;
	}

	return add_var(state, c_name, a_value + b_value);
}

//unsafe
bool execute_C_MUL(commands_state* state) {
	char* a_name = state->args[0].var_a_name;
	char* b_name = state->args[1].var_a_name;
	char* c_name = state->args[2].var_a_name;

	int success;
	int a_value = get_var_value_by_name(state, a_name, &success);
	if (success < 0) {
		if (ALLOW_UNDECLARED) {
			bool add_var_succ = add_var(state, a_name, state->default_init_value);
			if ( !add_var_succ ) {
				return false;
			}

			a_value = state->default_init_value;
		} else {
			return false;
		}
	}

	int b_value = get_var_value_by_name(state, b_name, &success);
	if (success < 0) {
		if (ALLOW_UNDECLARED) {
			bool add_var_succ = add_var(state, b_name, state->default_init_value);
			if ( !add_var_succ ) {
				return false;
			}		

			b_value = state->default_init_value;
		} else {
			return false;
		}
	}	

	get_var_value_by_name(state, c_name, &success);
	
	if (success < 0 && !ALLOW_UNDECLARED) {
		return false;
	}

	if (INT_MAX / a_value < b_value || INT_MAX / b_value < a_value) {
		return false;
	}

	return add_var(state, c_name, a_value * b_value);
}

//unsafe
bool execute_C_DIV(commands_state* state) {
	char* a_name = state->args[0].var_a_name;
	char* b_name = state->args[1].var_a_name;
	char* c_name = state->args[2].var_a_name;

	int success;
	int a_value = get_var_value_by_name(state, a_name, &success);
	if (success < 0) {
		if (ALLOW_UNDECLARED) {
			bool add_var_succ = add_var(state, a_name, state->default_init_value);
			if ( !add_var_succ ) {
				return false;
			}
			
			a_value = state->default_init_value;
		} else {
			return false;
		}
	}

	int b_value = get_var_value_by_name(state, b_name, &success);
	if (success < 0) {
		if (ALLOW_UNDECLARED) {
			bool add_var_succ = add_var(state, b_name, state->default_init_value);
			if ( !add_var_succ ) { 
				return false;
			}

			b_value = state->default_init_value;
		} else {
			return false;
		}
	}

	if (b_value == 0) {
		return false;
	}

	get_var_value_by_name(state, c_name, &success);
	
	if (success < 0 && !ALLOW_UNDECLARED) {
		return false;
	}

	return add_var(state, c_name, a_value / b_value);
}

//unsafe
bool execute_C_SUB(commands_state* state) {
	char* a_name = state->args[0].var_a_name;
	char* b_name = state->args[1].var_a_name;
	char* c_name = state->args[2].var_a_name;

	int success;
	int a_value = get_var_value_by_name(state, a_name, &success);
	if (success < 0) {
		if (ALLOW_UNDECLARED) {
			bool add_var_succ = add_var(state, a_name, state->default_init_value);
			if ( !add_var_succ ) {
				return false;
			}

			a_value = state->default_init_value;
		} else {
			return false;
		}
	}
	
	int b_value = get_var_value_by_name(state, b_name, &success);
	if (success < 0) {
		if (ALLOW_UNDECLARED) {
			bool add_var_succ = add_var(state, b_name, state->default_init_value);
			if ( !add_var_succ ) {
				return false;
			}

			b_value = state->default_init_value;
		} else {
			return false;
		}
	}

	get_var_value_by_name(state, c_name, &success);

	if (success < 0 && !ALLOW_UNDECLARED) {
		return false;
	}

	if (INT_MAX - abs(a_value) < abs(b_value) || INT_MAX - abs(b_value) < abs(a_value)) {
		return false;
	}

	return add_var(state, c_name, a_value - b_value);
}

//unsafe
bool execute_C_JMP(commands_state* state) {
	return jump_to_line(state->args[0].var_a_int);	
}

//unsafe
bool execute_C_CMP(commands_state* state) {
	char* a_name = state->args[0].var_a_name;
	char* b_name = state->args[1].var_a_name;
	
	int success;
	int a_value = get_var_value_by_name(state, a_name, &success);
	if (success < 0) {
		if (ALLOW_UNDECLARED) {
			bool add_var_succ = add_var(state, a_name, state->default_init_value);
			if ( !add_var_succ ) {
				return false;
			}

			a_value = state->default_init_value;
		} else {
			return false;
		}
	}	

	int b_value = get_var_value_by_name(state, b_name, &success);
	if (success < 0) {
		if (ALLOW_UNDECLARED) {
			bool add_var_succ = add_var(state, b_name, state->default_init_value);
			if ( !add_var_succ ) {
				return false;
			}

			b_value = state->default_init_value;
		} else {
			return false;
		}
	}

	if (a_value < b_value) {
		return jump_to_line(state->args[2].var_a_int);
	} else if (a_value == b_value) {
		return jump_to_line(state->args[3].var_a_int);
	} else {
		return jump_to_line(state->args[4].var_a_int);
	}

	// ?
	return false;
}

//unsafe
bool execute_current_command(commands_state* state) {
	if (state == NULL || state->vars_h_table == NULL) {
		return false;
	}

	bool execute_result;
	switch(state->current_command) {
		case C_EMPTY:
			execute_result = false;
			break;
		case C_LET:
			execute_result = execute_C_LET(state);	
			break;
		case C_OUT:
			execute_result = execute_C_OUT(state);
			break;
		case C_MOV:
			execute_result = execute_C_MOV(state);
			break;
		case C_ADD:
			execute_result = execute_C_ADD(state);
			break;
		case C_MUL:
			execute_result = execute_C_MUL(state);
			break;
		case C_DIV:
			execute_result = execute_C_DIV(state);
			break;
		case C_SUB:
			execute_result = execute_C_SUB(state);
			break;
		case C_JMP:
			execute_result = execute_C_JMP(state);
			break;
		case C_CMP:
			execute_result = execute_C_CMP(state);
		default:
			execute_result = true;
	}

	clear_commands_state(state);
	
	return execute_result;
}

bool get_input_lines_info(commands_state* state) {
	if (state == NULL || state->input_lines_info == NULL || state->input == NULL) {
		return false;
	}

	FILE* input = state->input;
	dynamic_array* arr = state->input_lines_info;

	int c;
	int current_position = 0;
	int prev_position = 0;

	while ( (c = fgetc(input)) != EOF ) {
		if (c == '\n') {
			bool success = dynamic_array_push_back(arr, prev_position);
			if ( !success ) {
				return false;
			}

			prev_position = current_position;
		}

		current_position += 1;
	}

	fseek(input, 0, SEEK_SET);

	return true;
}
