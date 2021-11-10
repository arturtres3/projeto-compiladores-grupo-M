# Grupo M

# Eduardo Henrique Ferreira do Nascimento (00260846)
# Artur Tres do Amaral (00287682)

clean: parser
	rm *.o lex.yy.c parser.tab.c parser.tab.h
#clear
	
parser: objects
	gcc -O3 -o etapa6 lex.yy.o parser.tab.o main.o valor_lexico.o ast.o tabela.o tipos.o ILOC.o

objects: flex
	gcc -O3 -c lex.yy.c parser.tab.c main.c valor_lexico.c ast.c tabela.c tipos.c ILOC.c

flex: bison
	lex  scanner.l

bison:
	bison -d parser.y

run:
	./etapa6

mem:
	valgrind ./etapa6

mem_full:
	valgrind --leak-check=full ./etapa6
	
save: 
	./etapa6 > output.txt

py_script:
	python3 txt_to_diag.py  ./output.txt ./grafo.txt

#copiar conteudo de grafo.txt para https://dreampuf.github.io/GraphvizOnline/
