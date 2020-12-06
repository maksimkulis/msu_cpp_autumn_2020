CC = g++ --std=c++17 

all: tests

tests: threadpool.h tests.cpp
	$(CC) tests.cpp -o prog

test: tests
	./prog

clean:
	rm prog
