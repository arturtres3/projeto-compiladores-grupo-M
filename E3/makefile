# Grupo M

# Eduardo Henrique Ferreira do Nascimento (00260846)
# Artur Tres do Amaral (00287682)

clean: parser
	rm *.o lex.yy.c parser.tab.c parser.tab.h

parser: objects
	gcc -O3 -o etapa3 lex.yy.o parser.tab.o main.o valor_lexico.o ast.o

objects: flex
	gcc -O3 -c lex.yy.c parser.tab.c main.c valor_lexico.c ast.c

flex: bison
	lex  scanner.l

bison:
	bison -d parser.y

run:
	./etapa3
