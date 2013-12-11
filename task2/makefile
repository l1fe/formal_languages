all: flex bison build

flex:
	flex -o lex.yy.cpp java.l

bison:
	bison -l -o java.tab.cpp -d java.y

build:
	g++ lex.yy.cpp java.tab.cpp -std=c++0x -o jt

clean:
	rm java.tab.* lex.yy.cpp jt
