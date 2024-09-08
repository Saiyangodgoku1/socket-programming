/*
You are required to create a TCP server and client where the server listens for incoming connections
from the client. The client sends a message to the server, and the server responds by sending the same
message back to the client. The server operates continuously, handling multiple client connections,
while the client sends one message and receives a response before terminating.
*/
Server Code 

#include<stdio.h>          // For standard I/O functions
#include<string.h>         // For string handling functions
#include<sys/socket.h>     // For socket-related functions
#include<netinet/in.h>     // For struct sockaddr_in (IP addresses)
#include<unistd.h>         // For POSIX API like close, fork, etc.

#define PORT 8080          // Port on which the server listens

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    char buffer[1024];

    // Step 1: Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Initialize server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    servaddr.sin_port = htons(PORT);        // Use the defined port

    // Step 3: Bind the socket to the server address
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Step 4: Listen for incoming connections
    listen(sockfd, 5);  // Allow up to 5 queued connection requests

    // Step 5: Continuously handle client connections
    while (1) {
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);  // Accept client connection
        
        read(newsockfd, buffer, sizeof(buffer));   // Read the message from the client
        write(newsockfd, buffer, strlen(buffer));  // Echo the message back to the client

        close(newsockfd);  // Close the client connection
    }

    // Step 6: Close the server socket (unreachable in this case)
    close(sockfd);
    return 0;
}

Client Code
#include<stdio.h>          // For standard I/O functions
#include<string.h>         // For string handling functions
#include<sys/socket.h>     // For socket-related functions
#include<netinet/in.h>     // For struct sockaddr_in (IP addresses)
#include<arpa/inet.h>      // For inet_addr function
#include<unistd.h>         // For POSIX API like close

#define PORT 8080          // Port on which the server is listening

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];

    // Step 1: Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Initialize server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost IP
    servaddr.sin_port = htons(PORT);

    // Step 3: Connect to the server
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Step 4: Send a message to the server
    strcpy(buffer, "Hello, Server!");  // Example message
    write(sockfd, buffer, strlen(buffer));

    // Step 5: Receive the echoed message from the server
    read(sockfd, buffer, sizeof(buffer));

    // Step 6: Close the socket connection
    close(sockfd);
    return 0;
}

/*These codes create a simple TCP echo server and client.
The server continuously listens for client connections, reads a message,
and echoes it back to the client. 
The client sends a single message to the server and prints the echoed response.*/
