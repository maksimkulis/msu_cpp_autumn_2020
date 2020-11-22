CC = g++ --std=c++17

all: tests

tests: Vector.h tests.cpp
	$(CC) tests.cpp -o tests

test: tests
	./tests

clean:
	rm tests