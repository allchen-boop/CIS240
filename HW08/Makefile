loader.o : loader.c loader.h
	clang -c loader.c

LC4.o : LC4.c LC4.h
	clang -c LC4.c

trace.o : trace.c loader.h LC4.h
	clang -c trace.c

trace : trace.o loader.o LC4.o
	clang -Wall -o trace trace.o loader.o LC4.o

clean :
	rm -f trace *.o
