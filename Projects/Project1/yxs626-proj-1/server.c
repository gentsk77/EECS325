/*--------------------------------------------------------------------
  EECS 325, Spring 2019
  Yue Shu, yxs626
  Project 1
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
main(int argc, char *argv[]) {
  int serversock;    						/* server socket*/
  fd_set liveskset, tempset;   	/* set of live client sockets */
  int liveskmax;   							/* maximum socket */

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
  
  /* init the live client set and its temp set to call select() on */
  FD_ZERO(&liveskset);
	FD_ZERO(&tempset);
	/* add server socket to the live socket set */
	FD_SET(serversock, &liveskset);  
	liveskmax = serversock;

  /* receive and process requests */
  while (1) {
		tempset = liveskset; 							/* copy the master set */
    int itsock;          							/* loop variable */

    /* using select() to serve both live and new clients */
	  if (select(liveskmax + 1, &tempset, NULL, NULL, NULL) == -1) {
		  error("ERROR on select");
			exit(4);
		}

    /* process messages from clients */
    for (itsock = 3; itsock <= liveskmax; itsock++) {
      /* skip the listen socket */
      if (itsock == serversock) continue;

      /* if receive message from client */
      if (FD_ISSET(itsock, &tempset)) {
				char *  clienthost;  						/* host name of the client */
				ushort  clientport;  						/* port number of the client */
	
				/* obtain client's host name and port	using getpeername() and gethostbyaddr() */
			  struct sockaddr_in clientaddr; 
				bzero(&clientaddr, sizeof(clientaddr));
				int client_len = sizeof(clientaddr);

		    /* get client's address */
				if (getpeername(itsock, (struct sockaddr*)&clientaddr, (socklen_t*)&client_len) == -1) {
					error("getpeername() failed");
					return -1;
				}

				/* get client's port number */
				clientport = ntohs(clientaddr.sin_port);
				struct hostent *client;
				client = gethostbyaddr((const char*)&clientaddr.sin_addr, 
																sizeof(clientaddr.sin_addr), AF_INET);
				
				/* get client's host name */
				clienthost = client->h_name;
				
				/* read the message */
				char * msg = recvdata(itsock);

				/* if client has exited */
				if (!msg) {
					/* disconnect from client */
					printf("admin: disconnect from '%s(%hu)'\n", clienthost, clientport);

					/* remove this client from 'liveskset', close the socket */
				  FD_CLR(itsock, &liveskset);
					close(itsock);
					free(msg);
				}

				/* if received message from live client */
				else {
					/* send the message to other clients through a loop */
				  int j;
				  for (j = 3; j <= liveskmax; j++) {
						if (FD_ISSET(j, &liveskset)) {
							if (j != serversock && j != itsock) 
								senddata(j, msg);
						}
					}
	  
					/* print the message on the server site */
					printf("%s(%hu): %s", clienthost, clientport, msg);
					free(msg);
				}
      }
    }

		/* if new client has connected */
    if (FD_ISSET(serversock, &tempset)) {
      /* accept the new connection request */
		  struct sockaddr_in clientaddr;
		  bzero(&clientaddr, sizeof(clientaddr));
      socklen_t clilen = sizeof(clientaddr);
      int newsockfd = accept(serversock, (struct sockaddr *) &clientaddr, &clilen);

		 	/* if connection successfully established */
      if (newsockfd >= 0) {
				char *  clienthost;  /* host name of the client */
				ushort  clientport;  /* port number of the client */

				/* get client's host name and port using gethostbyaddr() */
			  clientport = ntohs(clientaddr.sin_port);
			  struct hostent *client;
				client = gethostbyaddr((const char*)&clientaddr.sin_addr, 
																sizeof(clientaddr.sin_addr), AF_INET);
				/* get client's host name */
				clienthost = client->h_name;
				printf("admin: connect from '%s' at '%hu'\n", clienthost, clientport);

				/* add the new client to the 'liveskset' */
			  FD_SET(newsockfd, &liveskset);
				if (newsockfd > liveskmax)
				  liveskmax = newsockfd;
			} 
			else {
				perror("accept");
				exit(0);
      }
    }
  }
}
/*--------------------------------------------------------------------*/
