CC = gcc
CFLAGS = -glldb -c -Wall -std=c99

run: dir error util heap graph main
	$(CC) -lm -o proj2 build/error.o build/util.o build/heap.o build/graph.o build/main.o

dir:
	mkdir -p build

main: src/main.c
	$(CC) $(CFLAGS) -o build/main.o src/main.c

util: src/util.h src/util.c
	$(CC) $(CFLAGS) -o build/util.o src/util.c

heap: src/heap.h src/heap.c
	$(CC) $(CFLAGS) -o build/heap.o src/heap.c

error: src/error.h src/error.c
	$(CC) $(CFLAGS) -o build/error.o src/error.c

graph: src/graph.h src/graph.c
	$(CC) $(CFLAGS) -o build/graph.o src/graph.c

clean:
	rm build/*.o
