/*--------------------------------------------------------------------
  EECS 325, Spring 2019
  Yue Shu, yxs626
  Project 2
  conference server 
	--------------------------------------------------------------------*/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h>
#include <time.h> 
#include <errno.h>
#include <stdlib.h>

extern char *  recvdata(int sd);
extern int     senddata(int sd, char *msg);
extern int     startserver();
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/

/* main function*/
main(int argc, char *argv[]) {
  int serversock;    /* server socket*/

  fd_set liveskset, tempset;   /* set of live client sockets */
  int liveskmax;   /* maximum socket */
	int maxclient = 5;  /* we allow at most 5 clients */
	int clients[maxclient];  /* the clients */

  /* check usage */
  if (argc != 1) {
    fprintf(stderr, "usage : %s\n", argv[0]);
    exit(1);
  }

  /* get ready to receive requests */
  serversock = startserver();
  if (serversock == -1) {
    exit(1);
  }
  
  /*
    TODO:
    init the live client set 
  */
  FD_ZERO(&liveskset);
	FD_SET(serversock, &liveskset);  /* add server socket to the live socket set */
	liveskmax = serversock;

  /* receive and process requests */
  while (1) {
		tempset = liveskset; /* copy the master set */
    int itsock;      /* loop variable */

    /*
      TODO:
      using select() to serve both live and new clients
    */
	  if (select(liveskmax + 1, &tempset, NULL, NULL, NULL) == -1) {
		  error("ERROR on select");
			exit(4);
		}

    /* process messages from clients */
    for (itsock = 3; itsock <= liveskmax; itsock++) {
      /* skip the listen socket */
      if (itsock == serversock) continue;
      /* TODO: message from client */
      if (FD_ISSET(itsock, &tempset)) {

				char *  clienthost;  /* host name of the client */
				ushort  clientport;  /* port number of the client */
	
				/*
					TODO:
					obtain client's host name and port
					using getpeername() and gethostbyaddr()
				*/
			  struct sockaddr_in clientaddr;
				bzero(&clientaddr, sizeof(clientaddr));
				int client_len = sizeof(clientaddr);
				if (getpeername(itsock, (struct sockaddr*)&clientaddr, (socklen_t*)&client_len) == -1) {
					error("getpeername() failed");
					return -1;
				}
				clientport = ntohs(clientaddr.sin_port);
				printf("admin: msg from  at '%hu'\n", clientport);
				/*
				struct hostent *client;
				client = gethostbyaddr(&clientaddr, client_len, AF_INET);
				clienthost = client->h_name;
				/*
				
				/* read the message */
				char * msg = recvdata(itsock);

				/* if client has exited */
				if (!msg) {
					/* disconnect from client */
					printf("admin: disconnect from '%hu'\n", clientport);

					/*
						TODO:
						remove this client from 'liveskset'  
					*/
				  FD_CLR(itsock, &liveskset);

					close(itsock);
				}

				/* if received message from live client */
				else {
					/*
						TODO:
						send the message to other clients
					*/
				  int j;
				  for (j = 3; j <= liveskmax; j++) {
						if (FD_ISSET(j, &liveskset)) {
							if (j != serversock && j != itsock) 
								senddata(j, msg);
						}
					}
	  
					/* print the message on the server site*/
					printf("%hu: %s", clientport, msg);
					free(msg);
				}
      }
    }

		/* if new client has connected */
		/* TODO: connect request from a new client */
    if (FD_ISSET(serversock, &tempset)) {

      /*
				TODO:
				accept a new connection request
      */
		  struct sockaddr_in clientaddr;
		  bzero(&clientaddr, sizeof(clientaddr));
      socklen_t clilen = sizeof(clientaddr);
      int newsockfd = accept(serversock, (struct sockaddr *) &clientaddr, &clilen);

		 	/* if connection successfully established */
			/* TODO: if accept is fine */
      if (newsockfd >= 0) {
				char *  clienthost;  /* host name of the client */
				ushort  clientport;  /* port number of the client */

				/*
					TODO:
					get client's host name and port using gethostbyaddr() 
				*/
			  clientport = ntohs(clientaddr.sin_port);
				/*
			  struct hostent *client;
				client = gethostbyaddr(&clientaddr, clilen, AF_INET);
				clienthost = client->h_name;
				*/
				printf("admin: connect from  at '%hu'\n", clientport);

				/*
					TODO:
					add this client to 'liveskset'
				*/
			  FD_SET(newsockfd, &liveskset);
				clienthost = &client->h_name;
				
				printf("admin: connect from  at '%hu'\n", clientport);
			} 
			else {
				perror("accept");
				exit(0);
      }
    }
  }
}
/*--------------------------------------------------------------------*/
