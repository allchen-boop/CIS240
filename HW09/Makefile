token.o : token.c token.h
	clang -c token.c

stack.o : stack.c stack.h
	clang -c stack.c

jc.o : jc.c token.h stack.h
	clang -c jc.c

jc : jc.o token.o stack.o
	clang -Wall -o jc jc.o token.o stack.o

clean :
	rm -f jc *.o
