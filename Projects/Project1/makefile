.c.o:
	gcc -g -c $?

# compile client and server
all: server

# compile client 
# client: client.o utils.o
#	gcc -g -o client client.o  utils.o

# compile server 
server: server.o utils.o
	gcc -g -o server server.o  utils.o