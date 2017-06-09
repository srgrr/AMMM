
FLAGS=-O3 -std=c++11 -Iinclude -Wall -fopenmp
SRC=src
OBJ=obj

${OBJ}/%.o: ${SRC}/%.cc
	g++ -c -o $@ $< ${FLAGS}

_OBJECTS=main.o solver.o bruteforce.o grasp.o brkga.o
OBJECTS=$(patsubst %,$(OBJ)/%, $(_OBJECTS))

all: ${OBJECTS}
	g++ -o main ${OBJECTS} ${FLAGS}
