#include "nytwd.h"
#include "socket.c"

char *PORTNUM;
int CONNECTIONS;
int CUR_CONNECTIONS;


int main(int argc, char *argv[])
{
	debug("Start of main\n");
	
	// Initialize variables
	
	int sockfd, newsockfd;  // listen on sockfd, accept connections to newfd
	struct sigaction sa;

	check(!parse_config(), "Server could not start, error in config file");

	check((sockfd = open_socket()) != -1, "Error opening socket");

	set_sigaction(&sa);

	
	log_info("server: waiting for connections...\n");

	while(1)
	{

		struct sockaddr_storage their_addr;
		socklen_t sin_size = sizeof their_addr;
		char ipaddr[INET6_ADDRSTRLEN];

		newsockfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if(newsockfd == -1)
		{
			log_err("Could not accept new connection");
		}
		else
		{
			CUR_CONNECTIONS++;
			inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), ipaddr, sizeof ipaddr);

			log_info("Accepted connection from %s", ipaddr);

			if (!fork())
			{
				debug("Forked into child process");
				close(sockfd);
				child_process(newsockfd);
			}

			close(newsockfd);
		}
			

	}

	return 0;

		


error:
	return -1;
}

int parse_config()
{
	debug("Start of parse_config()\n");

	PORTNUM = "80";
	CONNECTIONS = 10;
	CUR_CONNECTIONS = 0;

	return 0;
}

void set_sigaction(struct sigaction *sa)
{
	debug("Start of set_sigaction()\n");

	sa->sa_handler = sigchild_handler;
	sigemptyset(&(sa->sa_mask));
	sa->sa_flags = SA_RESTART;
	
	int rtn = sigaction(SIGCHLD, sa, NULL);

	if(rtn == -1)
	{
		log_err("Error on sigaction");
		exit(1);
	}
}


void sigchild_handler(int s)
{
	// Save errno incase waitpid() overwrites it
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);
	
	CUR_CONNECTIONS--;
	
	debug("Child process ended");

	errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void child_process(int newsockfd)
{
	read_request(newsockfd);
	send_response(newsockfd, construct_response(200));
	close(newsockfd);
	exit(0);
}

