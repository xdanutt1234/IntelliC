CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS= -L/usr/local/lib64 -lws

websocket: websocket.o libaux.a
	$(CC) $(CFLAGS) websocket.o -o server -L. -laux $(LDFLAGS)

websocket.o: websocket.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c websocket.c

aux.o: aux.c
	$(CC) $(CFLAGS) -c aux.c

libaux.a: aux.o
	ar rcs libaux.a aux.o

clean:
	rm -f websocket.o aux.o libaux.a compiled-programs/*

.PHONY: clean
