#include <stdbool.h>

#include "command_types.h"
#include "static_hash_table.h"
#include "state.h"
#include "utils.h"
#include <string.h>

commands_state* create_commands_state() {
	commands_state* state = (commands_state*)malloc(sizeof(commands_state));
	if (state == NULL) {
		abort_prg("create_commands_state: malloc error");
	}

	state->current_command = C_EMPTY;
	state->current_command_args_num = 0;
	state->vars_h_table = static_hash_table_create();
	state->current_arg_index = 0;

	int i;
	for (i = 0; i < MAX_ARGS_NUM; ++i) {
		state->args[i].argument_type = A_EMPTY;
		state->args[i].value = NULL;
	}

	return state;
}

command_t get_current_command(commands_state state) {
	return state.current_command;
}

unsigned short get_current_command_args_num(commands_state state) {
	return state.current_command_args_num;
}

bool set_current_command(commands_state* state, command_t command, int command_args_num) {
	if (state == NULL || command_args_num < 0) {
		return false;
	}

	state->current_command = command;
	state->current_command_args_num = command_args_num;

	return true;
}

int get_var_value_by_name(commands_state* state, char* var_name, int* ind) {
	if (state == NULL || state->vars_h_table == NULL || var_name == NULL) {
		*ind = -1;
		return -1;
	}
	
	int* ht_get_result = (int*)static_hash_table_get(state->vars_h_table, var_name);
	if (ht_get_result == NULL) {
		*ind = -1;
		return -1;
	}

	return *ht_get_result;
}

bool set_var_value(commands_state* state, char* var_name, int value) {
	if (state == NULL || state->vars_h_table == NULL || var_name == NULL) {
		return false;
	}

	state->vars_h_table = static_hash_table_put(state->vars_h_table, var_name, strlen(var_name), &value, sizeof(int));
	
	if (state->vars_h_table == NULL) {
		abort_prg("set_var_value: hash_table_put error");
	}

	return true;
}

bool add_var(commands_state* state, char* var_name, int value) {
	if (state == NULL || state->vars_h_table == NULL || var_name == NULL) {
		return false;
	}

	state->vars_h_table = static_hash_table_put(state->vars_h_table, var_name, strlen(var_name), &value, sizeof(int));

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

		if (state->args[i].value == NULL) {
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

bool add_command_argument(commands_state* state, void* value, argument_t argument_type) {
	if (state == NULL || state->vars_h_table == NULL || value == NULL) {
		return false;
	}
	
	if (state->current_arg_index >= MAX_ARGS_NUM) {
		return false;
	}

	argument_struct argument_to_add;
	argument_to_add.value = value;
	argument_to_add.argument_type = argument_type;
	state->args[state->current_arg_index] = argument_to_add;
	state->current_arg_index += 1;

	return true;
}

void clear_commands_state(commands_state* state) {
	if (state == NULL || state->vars_h_table == NULL) {
		return;
	}

	state->current_command = C_EMPTY;
	state->current_command_args_num = 0;
	static_hash_table_clear(state->vars_h_table);
	
	unsigned short i;
	for (i = 0; i < MAX_ARGS_NUM; ++i) {
		state->args[i].value = NULL;
		state->args[i].argument_type = A_EMPTY;
	}

	state->current_arg_index = 0;
}

bool execute_current_command(commands_state* state) {
	if (state == NULL || state->vars_h_table == NULL) {
		return false;
	}

	printf("executing cmd.. OK\n");
	clear_commands_state(state);
	return true;
}
