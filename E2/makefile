clean: parser
	rm *.o lex.yy.c parser.tab.c parser.tab.h

parser: lex.yy.o 
	gcc -O3 -o parser lex.yy.o parser.tab.o main.o
	
lex.yy.o: lex.yy.c 
	gcc -O3 -c lex.yy.c parser.tab.c main.c
	
lex.yy.c: bison
	lex  scanner.l 
	
bison: scanner.l
	bison -d parser.y
	

