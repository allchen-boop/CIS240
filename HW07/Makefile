loader.o : loader.c loader.h
	clang -c loader.c

LC4.o : LC4.c LC4.h
	clang -c LC4.c

parser.o : parser.c loader.h LC4.h
	clang -c parser.c

parser : parser.o loader.o LC4.o
	clang -Wall -o parser parser.o loader.o LC4.o
