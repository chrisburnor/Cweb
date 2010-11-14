/* 
 * cweb.c
 * 
 * Minimal server written by Christopher Burnor
 *
 */

#include <stdio.h>

/* Parse arguments */
void parseArgs(void) {

}

/* handle errors */
void errorHandler(char *message) {

}

/* Create the connection and return file descriptor to socket */
int createServerConnection(void) {
	int connectionStatus, sd;
	struct addrinfo connectionParams;
  struct addrinfo *serverInfo;

	/* Set all options in connectionParams to 0 initially */
  memset(&connectionParams, 0, sizeof(connectionParams));
	/* Set actual paramaters for connection */
	connectionParams.ai_family   = AF_UNSPEC;   /* IPV4/IPV6 agnostic */
	connectionParams.ai_socktype = SOCK_STREAM; /* USE TCP */
	connectionParams.ai_flags    = AI_PASSIVE;  /* Use own IP addres since this is the server's connection info */

	connectionStatus = getaddrinfo(NULL, PORT, &connectionParams, &serverInfo);	
	if (connectionStatus == -1) {
		errorHandler("Could not get server address info");	
	}

	/* So really what should happen hear is that I loop over the linked list returned by getaddrinfo in serverInfo */
	
	sd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if (sd == -1) {
		errorHandler("Could not connect to socket");
	}
	bind(sd, serverInfo->ai_addr, serverInfo->ai_addrlen);
	if (listen(sd, MAX_BACKLOG_ITEMS) != 0) {
		errorHandler("Could not listen");
	}

	return sd;
}

/* Handles incoming requests */
void connectionHandler(int sd) {

}

/* Closes the connection */
void closeConnection(void) {

}


int main() {
	int sd;

	sd = createConnection();
  listen(sd);
   accept();
}
