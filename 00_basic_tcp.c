/*Explanation of the Code Structure
TCP:

Server:
Creates a TCP socket using socket().
Binds the socket to a specific port using bind().
Listens for incoming connections using listen().
Accepts a connection from a client using accept().
Reads data from the client, processes it, and sends a response.
Client:
Creates a TCP socket.
Connects to the server using connect().
TCP Server (General Syntax)
#include<stdio.h>          // Standard input/output functions
#include<string.h>         // String handling functions
#include<stdlib.h>         // Standard library functions
#include<sys/socket.h>     // Socket functions
#include<netinet/in.h>     // Contains structures for internet addresses
#include<arpa/inet.h>      // For handling internet operations
#include<unistd.h>         // POSIX API like close

#define PORTNO 8080        // Port number for the server to listen on

int main() {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in seraddr, cliaddr;
    char buffer[1024];     // Buffer for storing data from client
    int n;

    // Step 1: Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);  // SOCK_STREAM denotes TCP
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Step 2: Initialize the server address structure
    seraddr.sin_family = AF_INET;              // IPv4 protocol
    seraddr.sin_addr.s_addr = INADDR_ANY;      // Accept connections from any IP
    seraddr.sin_port = htons(PORTNO);          // Convert port number to network byte order

    // Step 3: Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    // Step 4: Listen for incoming connections
    listen(sockfd, 5);  // Allow up to 5 queued connections
    printf("Server waiting for client...\n");

    // Step 5: Accept a connection from a client
    clilen = sizeof(cliaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
    if (newsockfd < 0) {
        perror("Error accepting connection");
        exit(1);
    }

    // Step 6: Read data from the client
    bzero(buffer, 1024);  // Clear the buffer
    n = read(newsockfd, buffer, 1023);  // Read message from client
    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }
    printf("Client: %s\n", buffer);

    // Step 7: Write response to the client
    n = write(newsockfd, "Message received", 16);  // Send response to client
    if (n < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    // Step 8: Close the sockets
    close(newsockfd);  // Close client socket
    close(sockfd);     // Close server socket
    return 0;
}
TCP Client (General Syntax)
#include<stdio.h>          // Standard input/output functions
#include<string.h>         // String handling functions
#include<stdlib.h>         // Standard library functions
#include<sys/socket.h>     // Socket functions
#include<netinet/in.h>     // Contains structures for internet addresses
#include<arpa/inet.h>      // For handling internet operations
#include<unistd.h>         // POSIX API like close

#define PORTNO 8080        // Port number to connect to the server

int main() {
    int sockfd, n;
    struct sockaddr_in servaddr;
    char buffer[1024];     // Buffer for sending/receiving data

    // Step 1: Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);  // SOCK_STREAM denotes TCP
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Step 2: Initialize the server address structure
    servaddr.sin_family = AF_INET;             // IPv4 protocol
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server address (localhost)
    servaddr.sin_port = htons(PORTNO);         // Convert port number to network byte order

    // Step 3: Connect to the server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Step 4: Send data to the server
    printf("Enter message: ");
    bzero(buffer, 1024);  // Clear the buffer
    fgets(buffer, 1023, stdin);  // Get user input
    n = write(sockfd, buffer, strlen(buffer));  // Send message to server
    if (n < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    // Step 5: Read the response from the server
    bzero(buffer, 1024);  // Clear the buffer
    n = read(sockfd, buffer, 1023);  // Read response from server
    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }
    printf("Server: %s\n", buffer);

    // Step 6: Close the socket
    close(sockfd);  // Close the connection
    return 0;
}

Sends data to the server, receives a response, and closes the connection.*/
