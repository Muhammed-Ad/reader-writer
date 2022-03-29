all:
	gcc -Wall -std=c11 -o rwmain main.c readerwriter.c -l pthread

r:
	gcc -o rwmain main.c readerwriter.c -l pthread
	./rwmain
