CC = gcc
CFLAGS = -Wall -g

all: tema2

build: functiiCoada-L.o functiiStiva-L.o main.o
	$(CC) $(CFLAGS) $^ -o tema2

run: ./tema2

main.o: main.c TCoada.h TStiva.h
	$(CC) $(CFLAGS) -c $< -o $@

functiiCoada-L.o: functiiCoada-L.c TCoada.h TCoada-L.h
	$(CC) $(CFLAGS) -c $< -o $@

functiiStiva-L.o: functiiStiva-L.c TStiva.h TStiva-L.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o tema2


