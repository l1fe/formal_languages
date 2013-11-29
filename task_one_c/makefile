CC = gcc
CFLAGS = -Wall --std=c99
FLEX = flex
RM = rm -rf
OUTPUT = interpreter

all: $(OUTPUT)

$(OUTPUT): ./static_hash_table/static_hash_table.o ./state/state.o ./dynamic_array/dynamic_array.o ./utils/utils.o lex.yy.c
	$(CC) $^ -lfl -o $@

lex.yy.c: interpreter.l
	$(FLEX) interpreter.l

static_hash_table.o: ./static_hash_table/static_hash_table.c

dynamic_array.o: ./dynamic_array/dynamic_array.c

state.o: ./state/state.c

utils.o: ./utils/utils.c

clean:
	$(RM) $(OUTPUT)
	$(RM) lex.yy.c
	$(RM) ./static_hash_table/*.o ./state/*.o ./utils/*.o ./dynamic_array/*.o
