clean: parser
	rm *.o lex.yy.c parser.tab.c parser.tab.h

parser: objects
	gcc -O3 -o parser lex.yy.o parser.tab.o main.o
	
objects: flex 
	gcc -O3 -c lex.yy.c parser.tab.c main.c
	
flex: bison
	lex  scanner.l 
	
bison: 
	bison -d parser.y
	

