#ifndef __nytwd_h__
#define __nytwd_h__


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "dbg.h"

/* Define global variables that can be manipulated by a config file */

extern char *PORTNUM;
extern int CONNECTIONS;


/* End of global variable definitions */



// Define function to parse config file and set appropriate variables

int parse_config();



// Define function to handle zombie children

void sigchild_handler(int s);



// Define function to set the sigaction struct;

void set_sigaction(struct sigaction *sa);



// Define function to open a socket on the host
// return the file descriptor of the newly opened socket

int open_socket();


// Define function to return a pointer to an ipv4 or ipv6 address

void *get_in_addr(struct sockaddr *sa);


// Define the function to call once a process forks into a child

void child_process(int newsockfd);



// Define function to read HTTP request

char *read_request(int newsockfd);


// Define function to parse HTTP request

int parse_request(char *request);


// Define function to build HTTP response

char *construct_response(int code);


// Define function to send HTTP response
void send_response(int newsockfd);
#endif
