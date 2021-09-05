etapa1: lex.yy.o main.o
	gcc -Wall -Wextra -O3 -o etapa1 lex.yy.o main.o
main.o: main.c
	gcc -Wall -Wextra -O3 -c main.c
lex.yy.o: lex.yy.c
	gcc -Wall -Wextra -O3 -c lex.yy.c
lex.yy.c: scanner.l
	lex --header-file=lex.yy.h scanner.l 
clean:
	rm *.o lex.yy.c etapa1