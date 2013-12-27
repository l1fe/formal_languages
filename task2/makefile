all: flex bison build

flex:
	flex -o lex.yy.cpp java.l

bison:
	bison -l -o java.tab.cpp -d java.y

build:
	g++ lex.yy.cpp java.tab.cpp ./engine/types.cpp ./engine/class.cpp ./engine/code_block.cpp ./engine/expression.cpp ./engine/state.cpp ./engine/statement.cpp -std=c++0x -o jt

clean:
	rm java.tab.* lex.yy.cpp jt
