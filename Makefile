# Balaniuc Rodion 315CB

CFLAGS= -Wall -Werror

build:
	gcc -o tema2 main.c -std=c99

run: build
	./tema2 

clean:
	rm -f ./tema2

valgrind:
	valgrind --leak-check=yes --track-origins=yes ./tema2
