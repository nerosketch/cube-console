all: cube-console clean

cube-console: cube-console.o
	gcc cube-console.o -o cube-console -lm

cube-console.o: main.c
	gcc -c main.c -o cube-console.o

clean:
	rm -rf *.o
