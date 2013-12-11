#ifndef COMMAND_TYPES_H_INCLUDED_
#define COMMAND_TYPES_H_INCLUDED_

#define MAX_ARGS_NUM 5

typedef enum {C_LET, C_MOV, C_ADD, C_SUB, C_MUL, C_DIV, C_JMP, C_CMP, C_OUT, C_EMPTY} command_t;

typedef enum {A_NAME, A_INT, A_EMPTY} argument_t;

typedef struct argument_struct_ {
	char* var_a_name;
	int var_a_int;
	argument_t argument_type;	
} argument_struct;

#endif
