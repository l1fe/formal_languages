#ifndef STATE_H_INCLUDED_
#define STATE_H_INCLUDED_

#include <stdbool.h>

#include "command_types.h"
#include "static_hash_table.h"
#include "utils.h"

typedef struct commands_state_ {
	command_t current_command;
	unsigned short current_command_args_num;		
	static_hash_table* vars_h_table;
	argument_struct args[MAX_ARGS_NUM];
	unsigned short current_arg_index;
} commands_state;

commands_state* create_commands_state();
void clear_commands_state(commands_state* state);
command_t get_current_command(commands_state);
unsigned short get_current_command_args_num(commands_state);
bool set_current_command(commands_state*, command_t, int);
int get_var_value_by_name(commands_state*, char*, int*);
bool set_var_value(commands_state*, char*, int);
bool add_var(commands_state*, char*, int);
bool check_current_command_ready(commands_state*, int*);
bool add_command_argument(commands_state*, void*, argument_t);
bool execute_current_command(commands_state*); 
#endif
