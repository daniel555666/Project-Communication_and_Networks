all: my_wget

my_wget.o:
	gcc -c my_wget.c -Wall

parseurl.o:
	gcc -c parseurl.c -Wall

my_wget: my_wget.o parseurl.o
	gcc -o my_wget my_wget.o parseurl.o -Wall

clean:
	rm -f *.o my_wget