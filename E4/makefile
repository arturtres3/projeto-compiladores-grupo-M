# Grupo M

# Eduardo Henrique Ferreira do Nascimento (00260846)
# Artur Tres do Amaral (00287682)

clean: parser
	rm *.o lex.yy.c parser.tab.c parser.tab.h

parser: objects
	gcc -O3 -o etapa4 lex.yy.o parser.tab.o main.o valor_lexico.o ast.o symbol_table_management.o error_report.o symboltable.o types.o stack.o 

objects: flex
	gcc -O3 -c lex.yy.c parser.tab.c main.c valor_lexico.c ast.c symbol_table_management.c error_report.c symboltable.c types.c stack.c

flex: bison
	lex  scanner.l

bison:
	bison -d parser.y

run:
	./etapa4
	
save: 
	./etapa4 > output.txt

py_script:
	python3 txt_to_diag.py  ./output.txt ./grafo.txt

#copiar conteudo de grafo.txt para https://dreampuf.github.io/GraphvizOnline/
