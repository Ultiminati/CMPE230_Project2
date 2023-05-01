advcalc2ir: main.o concatenator.o lexer.o stacks.o
	gcc main.o concatenator.o lexer.o stacks.o -o advcalc2ir

main.o: main.c concatenator.h lexer.h stacks.h
	gcc -g -c main.c

lexer.o: lexer.c lexer.h
	gcc -g -c lexer.c

stacks.o: stacks.c stacks.h
	gcc -g -c stacks.c

concatenator.o: concatenator.c concatenator.h
	gcc -g -c concatenator.c
