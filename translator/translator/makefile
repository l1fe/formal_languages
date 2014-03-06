all: flex bison build

flex:
	flex -o lex.yy.cpp -d java.l

bison:
	bison -v -o java.tab.cpp -d java.y

build:
	g++ lex.yy.cpp java.tab.cpp translator.cpp gen.cpp tab.cpp engine/expression.cpp engine/statement.cpp -o jt

clean:
	rm jt java.output java.tab.cpp java.tab.hpp
