CC = g++ --std=c++17

all: tests

tests: tests.cpp format.h
	$(CC) tests.cpp -o tests

test: tests
	@./tests

clean:
	@rm -f tests