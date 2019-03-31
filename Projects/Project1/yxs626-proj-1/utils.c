/*--------------------------------------------------------------------
  EECS 325, Spring 2019
  Yue Shu, yxs626
  Project 1
  functions to connect clients and server
	--------------------------------------------------------------------*/
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h>
#include <time.h> 
#include <errno.h>

#define MAXNAMELEN 256
/*--------------------------------------------------------------------*/

/*----------------------------------------------------------------*/

/* wrapper method for error msg */
void error(const char *msg) {
    perror(msg);
    exit(1);
}

/* initialize the server */
int startserver() {
  int     sd;        /* socket */
  char    serverhost[128];  /* hostname */
  ushort  serverport;  /* server port */ 
  
  /* create a TCP socket */
  sd = socket(PF_INET, SOCK_STREAM, 0);
  if (sd < 0) 
    error("ERROR opening socket.");

  /* bind the socket to some random port, chosen by the system */
  struct sockaddr_in serveraddr;
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = INADDR_ANY;
  /* input 0 indicates that the system will randomly assign an available port */
  serveraddr.sin_port = htons(0);
  if (bind(sd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    error("ERROR on binding");
  
  /* ready to receive connections */
  listen(sd, 5);

  /* obtain the full local host name (serverhost) with gethostname() and gethostbyname() */
  gethostname(serverhost, sizeof(serverhost));
  struct hostent *server;
  server = gethostbyname(serverhost);

  /* get the port assigned to this server (serverport) with getsockname() */
  struct sockaddr_in myaddr;
  bzero(&myaddr, sizeof(myaddr));
  int mylen = sizeof(myaddr);
  getsockname(sd, (struct sockaddr *) &myaddr, &mylen);
  serverport = ntohs(myaddr.sin_port);

  /* ready to accept requests */
  printf("admin: started server on '%s' at '%hu'\n",
	serverhost, serverport);
  return(sd);
}

/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/

/* establishes connection with the server */
int connecttoserver(char *serverhost, ushort serverport) {
  int     sd;          /* socket */
  ushort  clientport;  /* port assigned to this client */

  /* create a TCP socket */
  sd = socket(PF_INET, SOCK_STREAM, 0);
  if (sd < 0) 
    error("ERROR opening socket.");

  /* connect to the server on 'serverhost' at 'serverport' with gethostbyname() and connect() */
  struct hostent *server;
  server = gethostbyname(serverhost);
  if (server == NULL) {
    fprintf(stderr, "ERROR: no such host. \n");
    exit(0);
  }

  /* init and assign serveraddr */
  struct sockaddr_in serveraddr;
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serveraddr.sin_addr.s_addr, server->h_length);
  serveraddr.sin_port = htons(serverport);
  if (connect(sd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    error("ERROR connecting");
  
  /* get the port assigned to this client with getsockname() */
  struct sockaddr_in myaddr;
  bzero(&myaddr, sizeof(myaddr));
  int mylen = sizeof(myaddr);
  getsockname(sd, (struct sockaddr *) &myaddr, &mylen);
  clientport = ntohs(myaddr.sin_port);

  /* succesful. return socket */
  printf("admin: connected to server on '%s' at '%hu' thru '%hu'\n",
	serverhost, serverport, clientport);
  return(sd);
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
int readn(int sd, char *buf, int n) {

  int     toberead;
  char *  ptr;
  toberead = n;
  ptr = buf;
  
  while (toberead > 0) {
    int byteread;

    byteread = read(sd, ptr, toberead);
    if (byteread <= 0) {
      if (byteread == -1)
	perror("read");
      return(0);
    }
    
    toberead -= byteread;
    ptr += byteread;
  }
  return(1);
}

char *recvdata(int sd) {
  char *msg;
  long  len;
  
  /* get the message length */
  if (!readn(sd, (char *) &len, sizeof(len))) {
    return(NULL);
  }
  len = ntohl(len);

  /* allocate memory for message */
  msg = NULL;
  if (len > 0) {
    msg = (char *) malloc(len);
    if (!msg) {
      fprintf(stderr, "error : unable to malloc\n");
      return(NULL);
    }

    /* read the message */
    if (!readn(sd, msg, len)) {
      free(msg);
      return(NULL);
    }
  }
  
  return(msg);
}

int senddata(int sd, char *msg) {
  long len;

  /* write lent */
  len = (msg ? strlen(msg) + 1 : 0);
  len = htonl(len);
  write(sd, (char *) &len, sizeof(len));
  
  /* write message data */
  len = ntohl(len);
  if (len > 0)
    write(sd, msg, len);
  return(1);
}

/*----------------------------------------------------------------*/
