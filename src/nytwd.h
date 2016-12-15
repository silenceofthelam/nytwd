#ifndef __nytwd_h__
#define __nytwd_h__


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <signal.h>
#include <time.h>

#include "packet.h"
#include "../lib/socket.h"
#include "../lib/dbg.h"


/* Define global variables that can be manipulated by a config file */

extern char *PORTNUM;
extern int CONNECTIONS;
extern int CUR_CONNECTIONS;
extern struct timespec START;

/* End of global variable definitions */



// Define function to parse config file and set appropriate variables
int parse_config();

// Define function to set the sigaction struct;
void set_sigaction(struct sigaction *sa);

// Define function to handle zombie children
void sigchild_handler(int s);

// Define function to return a pointer to an ipv4 or ipv6 address
void *get_in_addr(struct sockaddr *sa);

// Define the function to call once a process forks into a child
void child_process(int newsockfd);

// Define function to parse HTTP request
//int parse_request(char *request);

// Return a pointer to a string that represents
// the uptime of the daemon.
char *get_uptime();

#endif
