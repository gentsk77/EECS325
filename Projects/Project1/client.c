/*--------------------------------------------------------------------
  EECS 325, Spring 2019
  Yue Shu, yxs626
  Project 2
  conference client 
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

#define MAXMSGLEN  1024

extern char *  recvdata(int sd);
extern int     senddata(int sd, char *msg);
extern int     connecttoserver(char *servhost, ushort servport);
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
main(int argc, char *argv[]) {
  int sock;
  fd_set master, tempset;  /* file descriptor set including server socket and input */
  int setmax;

  /* check usage */
  if (argc != 3) {
    fprintf(stderr, "usage : %s <servhost> <servport>\n", argv[0]);
    exit(1);
  }

  /* connect to the server */
  sock = connecttoserver(argv[1], atoi(argv[2]));
  if (sock == -1)
    exit(1);

  /* init the fd set */
  FD_ZERO(&master);
  FD_ZERO(&tempset);
  FD_SET(sock, &master);
  setmax = sock;
  FD_SET(fileno(stdin), &master);
  if (fileno(stdin) > setmax)
    setmax = fileno(stdin);
  
  while (1) {
    
    /*
      TODO: 
      use select() to watch for user inputs and messages from the server
    */
    tempset = master;
    if (select(setmax + 1, &tempset, NULL, NULL, NULL) == -1) {
      error("ERROR on select");
      exit(4);
    }

    /* TODO: message from server */
    if (FD_ISSET(sock, &tempset)) {
      char *msg = recvdata(sock);

      if (!msg) {
	      /* server died, exit */
	      fprintf(stderr, "error: server died\n");
	      exit(1);
      }

      /* print the message */
      printf(">>> %s", msg);
      free(msg);
    }

    if (FD_ISSET(fileno(stdin), &tempset)) {
      char msg[MAXMSGLEN];
      
      if (!fgets(msg, MAXMSGLEN, stdin))
	      exit(0);
      senddata(sock, msg);
    }
  }
}
/*--------------------------------------------------------------------*/
