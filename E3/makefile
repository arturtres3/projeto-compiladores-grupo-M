# Grupo M

# Eduardo Henrique Ferreira do Nascimento (00260846)
# Artur Tres do Amaral (00287682)

clean: parser
	rm *.o lex.yy.c parser.tab.c parser.tab.h

parser: objects
	gcc -O3 -o etapa3 lex.yy.o parser.tab.o mainTESTE.o valor_lexico.o
	
objects: flex 
	gcc -O3 -c lex.yy.c parser.tab.c mainTESTE.c valor_lexico.c
	
flex: bison
	lex  scanner.l 
	
bison: 
	bison -d parser.y
	
run:
	./etapa3

