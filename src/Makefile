CC=gcc
CFLAGS=-c -g
LFLAGS=-luv

all: server client

server: Server.o
	$(CC) Server.o $(LFLAGS) -o server

Server.o: Server.c Connection.h DPool.h Buffer.h Packet.h
	$(CC) $(CFLAGS) Server.c

client: Client.o
	$(CC) Client.o $(LFLAGS) -o client

Client.o: Client.c Connection.h DPool.h Buffer.h Packet.h
	$(CC) $(CFLAGS) Client.c

clean:
	rm *.o

clean_all:
	rm *.o server client
