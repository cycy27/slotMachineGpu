all: compile execute

compile: projet.o
	gcc -Wall projet.o -o exec

projet.o: projet.c
	gcc -Wall projet.c -c

execute: exec
	./exec

clear:
	rm *.o exec