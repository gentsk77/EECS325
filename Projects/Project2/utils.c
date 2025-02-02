/*--------------------------------------------------------------------
  EECS 325, Spring 2019
  Yue Shu, yxs626
  Project 2
  functions for proxy
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
#include <stdlib.h>
#include "utils.h"


#define MAXNAMELEN 256
#define MAXMSGLEN 8000
#define RESPMSGLEN 65536

/*--------------------------------------------------------------------*/

struct pair *searchpair(struct pair *head, int serversd) {
    struct pair *ptr;
    ptr = head->next;

    while (ptr) {
        if (ptr->serversd == serversd)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

int insertpair(struct pair *head, int serversd, int clientsd) {
    struct pair *ptr = head;

    while (ptr->next != NULL) {
        ptr = ptr->next;
    }

    ptr->next = malloc(sizeof(struct pair));
    ptr->next->serversd = serversd;
    ptr->next->clientsd = clientsd;
    ptr->next->next = NULL;
}

struct pair *deletepair(struct pair *head, int serversd) {
    struct pair *prev = head;
    struct pair *ptr = head->next;

    while (ptr) {
        if (ptr->serversd == serversd) {
            if (ptr->next)
                prev->next = ptr->next;
            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }
    return (ptr);
}

/*----------------------------------------------------------------*/
/* prepare server to accept requests
   returns file descriptor of socket
   returns -1 on error
*/
int startserver() {
    int sd; /* socket descriptor */

    char *servhost;  /* full name of this host */
    ushort servport; /* port assigned to this server */

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Can't create socket.\n");
        return -1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = 0;

    if (bind(sd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        fprintf(stderr, "Can't bind socket.\n");
        close(sd);
        return -1;
    }

    /* we are ready to receive connections */
    listen(sd, 5);

    char localhost[MAXNAMELEN];
    localhost[MAXNAMELEN - 1] = '\0';
    gethostname(localhost, MAXNAMELEN - 1);
    struct hostent *h;
    if ((h = gethostbyname(localhost)) == NULL) {
        fprintf(stderr, "Can't get host by name.\n");
        servhost = "NULL";
    }
    else
        servhost = h->h_name;

    socklen_t len = sizeof(struct sockaddr_in);
    if (getsockname(sd, (struct sockaddr *)&sin, &len) < 0) {
        fprintf(stderr, "Can't get servport.\n");
        servport = -1;
    }
    else {
        servport = ntohs(sin.sin_port);
    }

    /* ready to accept requests */
    printf("admin: started server on '%s' at '%hu' \n", servhost, servport);
    return (sd);
}

/*----------------------------------------------------------------*/
/*
  establishes connection with the server
  returns file descriptor of socket
  returns -1 on error
*/
int connecttoserver(char *servhost, ushort servport) {

    int sd; /* socket descriptor */

    ushort clientport; /* port assigned to this client */

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Can't create socket.\n");
        return -1;
    }

    struct sockaddr_in sin;
    bzero(&sin, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(servport);

    struct hostent *server;
    if (server = gethostbyname(servhost))
        memcpy(&sin.sin_addr, server->h_addr, server->h_length);
    else {
        fprintf(stderr, "connecttoserver: Can't get host by name.\n");
        return -1;
    }

    if (connect(sd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        fprintf(stderr, "Can't connect to server.\n");
        close(sd);
        return -1;
    }

    socklen_t len = sizeof(struct sockaddr_in);
    if (getsockname(sd, (struct sockaddr *)&sin, &len) < 0) {
        fprintf(stderr, "Can't get local port.\n");
        clientport = -1;
    }
    else {
        clientport = ntohs(sin.sin_port);
    }
    /* succesful. return socket descriptor */
    printf("admin: connected to server on '%s' at '%hu' thru '%hu'\n", servhost, servport, clientport);
    return (sd);
}
/*----------------------------------------------------------------*/

/* send request to http proxy */
int sendrequest(int sd) {
    int newsd;
    char *msg, *url, *servhost, *servport;
    char *msgcp;
    int len;

    msg = (char *)malloc(MAXMSGLEN);
    msgcp = (char *)malloc(MAXMSGLEN);
    if (!msg || !msgcp) {
        free(msg);
        free(msgcp);
        fprintf(stderr, "error : unable to malloc\n");
        return (1);
    }

    /* read the message text */
    len = read(sd, msg, MAXMSGLEN);
    if (!len) {
        free(msg);
        free(msgcp);
        return len;
    }

    memcpy(msgcp, msg, MAXMSGLEN);

    /* extract servhost and servport from http request */
    url = strtok(msgcp, " ");
    url = strtok(NULL, " ");
    servhost = strtok(url, "//");
    servhost = strtok(NULL, "/");
    servport = strtok(servhost, ":");
    servport = strtok(NULL, ":");
    if (!servport)
        servport = "80";

    if (servhost && servport) {
        /* TODO: establish new connection to http server on behalf of the user 
         * use connecttoserver() and write() */
        newsd = connecttoserver(servhost, atoi(servport));

        write(newsd, msg, MAXMSGLEN);

        free(msgcp);
        free(msg);
        /*TODO: return newly created socket file descriptor */
        return newsd;
    }
    return (0);
}

char *readresponse(int sd) {                                
    char *msg;
    char *ptr;
    long len;

    /* used to read the whole response msg */
    ptr = (char *)malloc(RESPMSGLEN);
    msg = (char *)malloc(RESPMSGLEN);

 
    if (!msg || !ptr) {
        fprintf(stderr, "error : unable to malloc\n");
        return (NULL);
    }

    int     toberead;
    toberead = RESPMSGLEN-1;
    ptr = msg;
    
    while (toberead > 0) {
        int byteread;

        byteread = read(sd, ptr, toberead);
        if (byteread < 0) {
                perror("read");
            return(0);
        }
        if (byteread == 0)
            break;
        
        toberead -= byteread;
        ptr += byteread;
    }
    
    return (msg);
}

/* Forward response message back to user */
void forwardresponse(int sd, char *msg) {
    write(sd, msg, RESPMSGLEN);
}

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