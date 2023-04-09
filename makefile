AdvCalc: main.o concatenator.o lexer.o stacks.o
	gcc main.o concatenator.o lexer.o stacks.o -o AdvCalc

main.o: main.c concatenator.h lexer.h stacks.h
	gcc -c main.c

lexer.o: lexer.c lexer.h
	gcc -c lexer.c

stacks.o: stacks.c stacks.h
	gcc -c stacks.c

concatenator.o: concatenator.c concatenator.h
	gcc -c concatenator.c