CC = g++ --std=c++17

all: tests

tests: main.o BigInt.o
	$(CC) main.o BigInt.o -o tests

main.o: main.cpp BigInt.h
	$(CC) -c main.cpp

BigInt.o: BigInt.cpp BigInt.h
	$(CC) -c BigInt.cpp

test: tests
	./tests

clean:
	rm *.o tests