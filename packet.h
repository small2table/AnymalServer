/*
	packet.h
	- packet and socket helper functions

	Author @ Juan Lee (juanlee@kaist.ac.kr)
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>

/* Constraints */
#define MAX_BUFFER_LENGTH 1024
#define MAX_PACKET_LENGTH 1024*1024*10
#define LISTENQ 1024
#define PACKET_ROW 64

/* Typedef Structure */
typedef struct sockaddr SA;

/* Socket Helper Function */
int open_listenfd(char *port);