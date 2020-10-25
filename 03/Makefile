CC = g++ --std=c++17

all: tests

tests: tests.o matrix.o
	$(CC) tests.o matrix.o -o tests

tests.o: tests.cpp matrix.h
	$(CC) -c tests.cpp

matrix.o: matrix.cpp matrix.h
	$(CC) -c matrix.cpp

test: tests
	./tests

clean:
	rm *.o tests