#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../command_types.h"
#include "../static_hash_table/static_hash_table.h"
#include "../state/state.h"
#include "../utils/utils.h"

commands_state* create_commands_state() {
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

	state->output = stdout;
	state->input = stdin;

	state->current_line = 0;

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
		bool add_var_a_success;
		add_var_a_success = add_var(state, a_name, state->default_init_value);

		if ( !add_var_a_success ) {
			return false;
		}

		a_value = state->default_init_value;
	}

	return add_var(state, b_name, a_value);
}

//unsafe
bool execute_C_OUT(commands_state* state) {
	char* a_name = state->args[0].var_a_name;

	int success;
	int a_value = get_var_value_by_name(state, a_name, &success);


	if (success < 0) {
		bool add_var_success = true;
		add_var_success = add_var(state, a_name, state->default_init_value);		
		if ( !add_var_success ) {
			return false;
		}
		
		a_value = state->default_init_value;
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
		bool add_var_succ = add_var(state, a_name, state->default_init_value);
		if ( !add_var_succ ) {
			return false;
		}
		
		a_value = state->default_init_value;
	}

	int b_value = get_var_value_by_name(state, b_name, &success);
	if (success < 0) {
		bool add_var_succ = add_var(state, b_name, state->default_init_value);
		if ( !add_var_succ ) {
			return false;
		}

		b_value = state->default_init_value;
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
		bool add_var_succ = add_var(state, a_name, state->default_init_value);
		if ( !add_var_succ ) {
			return false;
		}

		a_value = state->default_init_value;
	}

	int b_value = get_var_value_by_name(state, b_name, &success);
	if (success < 0) {
		bool add_var_succ = add_var(state, b_name, state->default_init_value);
		if ( !add_var_succ ) {
			return false;
		}		

		b_value = state->default_init_value;
	}	

	printf("result = %d\n", a_value * b_value);
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
		bool add_var_succ = add_var(state, a_name, state->default_init_value);
		if ( !add_var_succ ) {
			return false;
		}

		a_value = state->default_init_value;
	}

	int b_value = get_var_value_by_name(state, b_name, &success);
	if (success < 0) {
		bool add_var_succ = add_var(state, b_name, state->default_init_value);
		if ( !add_var_succ ) { 
			return false;
		}

		b_value = state->default_init_value;
	}

	return add_var(state, c_name, a_value / b_value);
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
		default:
			execute_result = true;
	}

	clear_commands_state(state);
	
	return execute_result;
}
