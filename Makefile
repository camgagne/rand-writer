CC = g++
CFLAGS = --std=c++20 -Wall -Werror -pedantic -g
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework

all: TextWriter.a TextWriter test

TextWriter: TextWriter.o RandWriter.o
	$(CC) $(CFLAGS) -o TextWriter TextWriter.o RandWriter.o $(LIB)

test: test.o RandWriter.o
	$(CC) $(CFLAGS) -o test test.o RandWriter.o $(LIB)

TextWriter.a: RandWriter.o
	ar rcs TextWriter.a RandWriter.o

TextWriter.o: TextWriter.cpp RandWriter.hpp
	$(CC) $(CFLAGS) -c TextWriter.cpp

RandWriter.o: RandWriter.cpp RandWriter.hpp
	$(CC) $(CFLAGS) -c RandWriter.cpp

test.o: test.cpp RandWriter.hpp
	$(CC) $(CFLAGS) -c test.cpp

clean:
	rm -f TextWriter test test.o TextWriter.o RandWriter.o TextWriter.a

lint:
	cpplint *.cpp *.hpp
