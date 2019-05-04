#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <errno.h>

#include "utils.h"

extern int sendrequest(int sd);
extern char *readresponse(int sd);
extern void forwardresponse(int sd, char *msg);
extern int startserver();

main(int argc, char *argv[]) {

	int servsock; /* server socket descriptor */
	fd_set livesdset, servsdset, tempset; /* set of live client sockets and set of live http server sockets */
	/* TODO: define largest file descriptor number used for select */
	int liveskmax;

	struct pair *table = malloc(sizeof(struct pair)); /* table to keep client<->server pairs */
	char *msg;

	/* check usage */
	if (argc != 1) {
		fprintf(stderr, "usage : %s\n", argv[0]);
		exit(1);
	}

	/* get ready to receive requests */
	servsock = startserver();
	if (servsock == -1) {
		exit(1);
	}

	table->next = NULL;

	/* TODO: initialize all the fd_sets and largest fd numbers */
	FD_ZERO(&livesdset);
	FD_ZERO(&servsdset);
	FD_SET(servsock, &livesdset);
	FD_SET(servsock, &servsdset);
	liveskmax = servsock;

	while (1) {

		int frsock;

		/* TODO: combine livesdset and servsdset, 
     	 * use the combined fd_set for select */
		int fd;
		fd_set combfdset;
		FD_ZERO(&combfdset);

		for (fd = 0; fd < FD_SETSIZE; fd++) {
			if (FD_ISSET(fd, &livesdset) || FD_ISSET(fd, &servsdset))
				FD_SET(fd, &combfdset);
		}

		tempset = combfdset;

		/* TODO: select from the combined fd_set */
		if (select(liveskmax + 1, &tempset, NULL, NULL, NULL) == -1) {
			fprintf(stderr, "Can't select.\n");
			continue;
		}

		for (frsock = 3; frsock <= liveskmax; frsock++) {
			if (frsock == servsock)
				continue;

			/* TODO: input from existing client? */
			if (FD_ISSET(frsock, &livesdset)) {
				/* forward the request */
				int newsd = sendrequest(frsock);
				if (!newsd) {
					printf("admin: disconnect from client\n");

					/*TODO: clear frsock from fd_set(s) */
					FD_CLR(frsock, &livesdset);
					close(frsock);
				}
				else {
					insertpair(table, newsd, frsock);

					/* TODO: insert newsd into fd_set(s) */
					FD_SET(newsd, &servsdset);

				}
			}
			/* TODO: input from the http server? */
			if (FD_ISSET(frsock, &servsdset)) {
				char *msg;
				struct pair *entry = NULL;
				struct pair *delentry;
				msg = readresponse(frsock);
				printf("im here2");
				if (!msg) {
					fprintf(stderr, "error: server died\n");
					exit(1);
				}

				/* forward response to client */
				entry = searchpair(table, frsock);
				if (!entry) {
					fprintf(stderr, "error: could not find matching clent sd\n");
					exit(1);
				}

				forwardresponse(entry->clientsd, msg);
				delentry = deletepair(table, entry->serversd);

				/* TODO: clear the client and server sockets used for 
		 		 * this http connection from the fd_set(s) */
				FD_CLR(entry->clientsd, &livesdset);
				FD_CLR(entry->serversd, &servsdset);
				close(entry->clientsd);
				close(entry->serversd);
			}
		}

		/* input from new client*/
		if (FD_ISSET(servsock, &tempset)) {
			struct sockaddr_in caddr;
			socklen_t clen = sizeof(caddr);
			int csd = accept(servsock, (struct sockaddr *)&caddr, &clen);

			if (csd != -1) {

				/* TODO: put csd into fd_set(s) */
				FD_SET(csd, &livesdset);
				if (csd > liveskmax)
					liveskmax = csd;
			}
			else {
				perror("accept");
				exit(0);
			}
		}
	}
}
