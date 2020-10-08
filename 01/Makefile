CC = g++ --std=c++17

all: program

program: my_allocator.o tests.o
	$(CC) my_allocator.o tests.o -o program

my_allocator.o: my_allocator.cpp my_allocator.h
	$(CC) -c my_allocator.cpp

tests.o: tests.cpp my_allocator.h
	$(CC) -c tests.cpp

test: program
	./program

clean:
	rm *.o program