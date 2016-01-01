CC=gcc
CFLAGS=-c -g
LFLAGS=-luv

all: server client

server: Server.o Connection.o
	$(CC) Server.o Connection.o $(LFLAGS) -o server

Server.o: Server.c Connection.h DPool.h Buffer.h Packet.h Type.h
	$(CC) $(CFLAGS) Server.c

client: Client.o Connection.o
	$(CC) Client.o Connection.o $(LFLAGS) -o client

Client.o: Client.c Connection.h DPool.h Buffer.h Packet.h Type.h
	$(CC) $(CFLAGS) Client.c

Connection.o: Connection.c Connection.h DPool.h Buffer.h Packet.h Type.h
	$(CC) $(CFLAGS) Connection.c

clean:
	rm *.o

clean_all:
	rm *.o server client
