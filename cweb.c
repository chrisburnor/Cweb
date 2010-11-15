/* 
 * cweb.c
 * 
 * Minimal server written by Christopher Burnor
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MY_PORT "8080"
#define MAX_BACKLOG_ITEMS 10
#define MAX_MESSAGE_SIZE 128 

/* handle errors */
void errorHandler(char *message) {
	fprintf(stderr, "Error: %s\n", message);
	exit(1);
}

/* Closes the connection */
void closeConnection(int sd) {
	int shutdownStatus;

	shutdownStatus = close(sd);
	if (shutdownStatus == -1) {
		errorHandler("Could not close socket: %i\n", sd);
	}
}

/* Handles individual requests */
void requestHandler(int sd) {
	int bytes_sent;
	char message[MAX_MESSAGE_SIZE] = "Hello Seattle, I am a chipmunk!";
	int message_length = strlen(message);

	bytes_sent = send(sd, message, message_length, 0);
	/* Need to insert error checking to make sure full message was sent and send rest if necessary */
	if (bytes_sent == -1) {
		errorHandler("Error sending message");
	}
}

/* Create the connection and return file descriptor to socket */
int createServerConnection(char *port) {
	/* sd is the socket descriptor for this connection. connectionStatus is simply a flag for testing succes */
	int connectionStatus, sd;
	struct addrinfo connectionParams;
  struct addrinfo *serverInfo;

	/* Set all options in connectionParams to 0 initially */
  memset(&connectionParams, 0, sizeof(connectionParams));
	/* Set actual paramaters for connection */
	connectionParams.ai_family   = AF_UNSPEC;   /* IPV4/IPV6 agnostic */
	connectionParams.ai_socktype = SOCK_STREAM; /* USE TCP */
	connectionParams.ai_flags    = AI_PASSIVE;  /* Use own IP addres since this is the server's connection info */

	connectionStatus = getaddrinfo(NULL, port, &connectionParams, &serverInfo);	
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
		errorHandler("Could not bind to socket");
	}
	
  connectionStatus = listen(sd, MAX_BACKLOG_ITEMS);
  if (connectionStatus == -1) {
		errorHandler("Could not listen on socket");
  }

	return sd;
}

/* Handles incoming requests */
void connectionHandler(int sd) {
	int new_sd;
	struct sockaddr_storage remote_addr;
	socklen_t addr_size;

	new_sd = accept(sd, (struct sockaddr *)&remote_addr, &addr_size);
	if (new_sd == -1) {
		errorHandler("Could not accept data from socket");
	}

	/* Pass control of the new socket descriptor to the requestHandler function which will handle the data of the reuset */
	requestHandler(new_sd);

	/* For right now, close the request immediately
	 * Eventually will want a loop here
	 */
	closeConnection(new_sd);
}

int main(void) {
	int sd;

	/* Set up the connection */
	sd = createServerConnection(MY_PORT);
	/* Listen on the socket defined by sd */
	connectionHandler(sd);
	closeConnection(sd);

	return 0;
}
