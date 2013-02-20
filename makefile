WARN = -Wall

all: server client

server: server.h server.c fonction.c fonction.h
	gcc $(WARN) server.c fonction.c -o server.exe -lpthread 

client: client.h client.c fonction.c fonction.h
	gcc $(WARN) client.c fonction.c -o client.exe -lpthread
