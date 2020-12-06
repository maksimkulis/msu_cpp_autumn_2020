CC = g++ --std=c++17 

all: tests

tests: threadpool.h tests.cpp
	$(CC) tests.cpp -o prog

run: tests
	./prog

clean:
	rm prog