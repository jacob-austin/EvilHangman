CC = gcc
CFLAGS = -Wall --std=c99 -g
OBJECTS = my_string.o vector.o avl.o main.o

string_driver: $(OBJECTS)
	$(CC) $(CFLAGS) -o string_driver $(OBJECTS)
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
my_string.o: my_string.h my_string.c
	$(CC) $(CFLAGS) -c my_string.h -o my_string.o
	$(CC) $(CFLAGS) -c my_string.c -o my_string.o
vector.o: vector.h vector.c
	$(CC) $(CFLAGS) -c vector.h -o vector.o
	$(CC) $(CFLAGS) -c vector.c -o vector.o
avl.o: avl.h avl.c
	$(CC) $(CFLAGS) -c avl.h -o avl.o
	$(CC) $(CFLAGS) -c avl.c -o avl.o
clean:
	rm string_driver $(OBJECTS)
