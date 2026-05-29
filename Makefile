CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g
LIBS = -lSDL2 -lSDL2_ttf -lm

all: programa

programa: lista.o masa.o resorte.o mallas.o main.o distancia.o simulacion.o matrices.o escrituraylectura.o
	$(CC) lista.o masa.o resorte.o mallas.o main.o distancia.o simulacion.o matrices.o escrituraylectura.o -o programa $(LIBS)

main.o: main.c mallas.h lista.h config.h masa.h resorte.h distancia.h simulacion.h matrices.h
	$(CC) $(CFLAGS) -c main.c

mallas.o: mallas.c mallas.h
	$(CC) $(CFLAGS) -c mallas.c

masa.o: masa.c masa.h
	$(CC) $(CFLAGS) -c masa.c

resorte.o: resorte.c resorte.h
	$(CC) $(CFLAGS) -c resorte.c

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c lista.c

distancia.o: distancia.c distancia.h
	$(CC) $(CFLAGS) -c distancia.c

simulacion.o: simulacion.c simulacion.h
	$(CC) $(CFLAGS) -c simulacion.c

matrices.o: matrices.c matrices.h
	$(CC) $(CFLAGS) -c matrices.c

escrituraylectura.o: escrituraylectura.c escrituraylectura.h
	$(CC) $(CFLAGS) -c escrituraylectura.c

clean:
	rm -f programa *.o
