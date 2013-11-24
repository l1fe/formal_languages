CC = gcc
CFLAGS = -Wall --std=c99
FLEX = flex
RM = rm -rf
OUTPUT = interpreter

all: $(OUTPUT)

$(OUTPUT): static_hash_table.o state.o utils.o lex.yy.c
	$(CC) $^ -lfl -o $@

lex.yy.c: interpreter.l
	$(FLEX) interpreter.l

static_hash_table.o: static_hash_table.c

state.o: state.c

utils.o: utils.c

clean:
	$(RM) $(OUTPUT)
	$(RM) lex.yy.c
	$(RM) *.o
