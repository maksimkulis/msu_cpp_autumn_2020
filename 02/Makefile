CC = g++ --std=c++17

all: tests

tests: tests.o TokenParser.o
	$(CC) tests.o TokenParser.o -o tests

tests.o: tests.cpp TokenParser.h
	$(CC) -c tests.cpp

TokenParser.o: TokenParser.cpp TokenParser.h
	$(CC) -c TokenParser.cpp

test: tests
	./tests

clean:
	rm *.o tests