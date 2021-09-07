# Grupo M

# Eduardo Henrique Ferreira do Nascimento (00260846)
# Artur Tres do Amaral (00287682)

clean: parser
	rm *.o lex.yy.c parser.tab.c parser.tab.h

parser: objects
	gcc -O3 -o etapa2 lex.yy.o parser.tab.o main.o
	
objects: flex 
	gcc -O3 -c lex.yy.c parser.tab.c main.c
	
flex: bison
	lex  scanner.l 
	
bison: 
	bison -d parser.y
	
run:
	./etapa2

