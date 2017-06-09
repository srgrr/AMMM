
FLAGS=-O3 -std=c++11 -Iinclude -Wall -fopenmp
SRC=src
OBJ=obj

${OBJ}/%.o: ${SRC}/%.cc
	g++ -c -o $@ $< ${FLAGS}

_SOURCES=main.cc solver.cc bruteforce.cc grasp.cc brkga.cc
SOURCES=$(patsubst %,$(SRC)/%, $(_SOURCES))

_OBJECTS=main.o solver.o bruteforce.o grasp.o brkga.o
OBJECTS=$(patsubst %,$(OBJ)/%, $(_OBJECTS))

all: ${OBJECTS} ${SOURCES} main
	g++ -o main ${OBJECTS} ${FLAGS}

clean:
	rm ${OBJ}/*.o
