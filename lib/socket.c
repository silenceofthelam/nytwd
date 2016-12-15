#include "socket.h"

int open_socket()
{
	debug("Start of open_socket\n");
	
	struct addrinfo hints, *servinfo;
	int yes = 1, rv, sockfd;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	check(!(rv = getaddrinfo(NULL, PORTNUM, &hints, &servinfo)), "getaddrinfo: %s\n", gai_strerror(rv));

	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	if(sockfd == -1)
	{
		log_warn("Could not create new socket");
	}

	check((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != -1), "Could not open socket");

	check((bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) != -1, "Could not bind socket to address");

	check((listen(sockfd, CONNECTIONS) != -1), "Could not listen on socket");

	check((getsockname(sockfd, servinfo->ai_addr, &(servinfo->ai_addrlen)) != -1), "Error getting socket information");

	log_info("Server listening on port %d\n", ntohs(((struct sockaddr_in *)(servinfo->ai_addr))->sin_port));

	freeaddrinfo(servinfo);

	return sockfd;

error:
	return -1;

}

char *read_request(int newsockfd)
{
	char *buffer = malloc(2048);
	check_mem(buffer);
	int rec = 0;
	int position = 0, EOR = 0; // EOR = End of Request

	memset(buffer, '\0', 2048);
	
	while(!EOR)
	{

		rec = recv(newsockfd, &(buffer[position]), (2047 - position), 0);

		debug("buffer so far:\n%s", buffer);
		debug("Bytes received: %d", rec);
		
		if(rec == 0)
		{
			debug("Client closed connection");
		
			return NULL;
		}

		position += rec;
		
				
		debug("char1: %d", buffer[position - 4]);
		debug("char2: %d", buffer[position - 3]);
		debug("char1: %d", buffer[position - 2]);
		debug("char2: %d", buffer[position - 1]);

		if((buffer[position - 4] == '\r') && (buffer[position - 3] == '\n') && (buffer[position - 2] == '\r') && (buffer[position - 1] == '\n'))
		{
			debug("End of request");
			EOR = 1;
		}

	}

	debug("Received %d bytes from client", (int)strlen(buffer));
	debug("Client said:\n%s", buffer);

	return buffer;

error:
	if(buffer) free(buffer);
	return NULL;
}



void send_response(int newsockfd, char *response)
{
	
	int len = strlen(response);

	int bytes_sent = send(newsockfd, response, len, 0);

	debug("Sent %d bytes to client", bytes_sent);

}
	
