.PHONY:all
all: Server Client
Client: Client.c
	gcc -Wall Client.c -o Client
Server: Server.c
	gcc -Wall Server.c -o Server
clean:
	rm -f Client Server
