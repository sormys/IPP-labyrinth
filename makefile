CC = gcc
CFLAGS = -Wall -Wextra -Wno-implicit-fallthrough -O2 -std=c17 -pedantic
objects = tools inputMng bfs queue bitOperations

all: labyrinth

labyrinth: $(objects:=.o)

labyrinth.o: inputMng.h tools.h bitOperations.h
tools.o: tools.h
queue.o: tools.h queue.h
bitOperations.o: bitOperations.h tools.h
bfs.o: bfs.h queue.h bitOperations.h
inputMng.o:  bfs.h inputMng.h tools.h bitOperations.h


clean: 
	rm -f *.o labyrinth
