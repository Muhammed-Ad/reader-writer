all:
	gcc -o rwmain main.c readerwriter.c -l pthread

r:
	gcc -o rwmain main.c readerwriter.c -l pthread
	./rwmain
