.PHONY:all
all: cproxy sproxy
Client: Client.c
	gcc -Wall Client.c -o Client
Server: Server.c
	gcc -Wall Server.c -o Server
cproxy: cproxy.c
	gcc -Wall cproxy.c -o cproxy
sproxy: sproxy.c
	gcc -Wall sproxy.c -o sproxy
clean:
	rm -f Client Server cproxy sproxy
