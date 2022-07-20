
OBJ = socket_client.o \
	socket_server.o

all: client server

client: socket_client.o
	gcc socket_client.o -o client

server: socket_server.o
	gcc socket_server.o -o server

clean:
	rm socket_server.o socket_client.o server client