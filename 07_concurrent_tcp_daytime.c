/*
Write a concurrent TCP daytime server program in C where the server sends the current date 
and time along with its process ID (PID) to the client. 
The server should be able to handle multiple clients concurrently using the fork() system call.*/

Server Code
#include<stdio.h>          // Standard input/output functions
#include<stdlib.h>         // Standard library functions
#include<string.h>         // String handling functions
#include<sys/socket.h>     // Socket functions
#include<netinet/in.h>     // Contains structures for internet addresses
#include<unistd.h>         // POSIX API for fork, close, etc.
#include<arpa/inet.h>      // For handling internet operations
#include<time.h>           // To get the current time
#include<signal.h>         // For signal handling to manage child processes

#define PORTNO 10200       // Port number the server will listen on
#define BUFFER_SIZE 1024   // Buffer size for outgoing messages

// Signal handler to reap zombie child processes
void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);  // Clean up child processes
}

int main() {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in seraddr, cliaddr;
    char buffer[BUFFER_SIZE];   // Buffer to store the message to send to the client
    int n;

    // Step 1: Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Step 2: Initialize the server address structure
    seraddr.sin_family = AF_INET;               // Use IPv4 protocol
    seraddr.sin_addr.s_addr = INADDR_ANY;       // Bind to all available interfaces
    seraddr.sin_port = htons(PORTNO);           // Convert port number to network byte order

    // Step 3: Bind the socket to the server's address and port
    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    // Step 4: Listen for incoming client connections
    listen(sockfd, 5);
    printf("Server waiting for client...\n");

    // Step 5: Set up signal handler to prevent zombie child processes
    signal(SIGCHLD, handle_sigchld);

    // Step 6: Accept client connections in an infinite loop
    while (1) {
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (newsockfd < 0) {
            perror("Error on accept");
            exit(1);
        }

        // Step 7: Fork a child process to handle the client
        if (fork() == 0) {
            // Child process

            // Step 8: Get the current date and time
            time_t current_time = time(NULL);   // Get the current time
            char *time_str = ctime(&current_time); // Convert time to string
            time_str[strlen(time_str) - 1] = '\0'; // Remove the newline character at the end

            // Step 9: Get the process ID of the current server instance
            pid_t pid = getpid();

            // Step 10: Prepare the message to send to the client
            snprintf(buffer, sizeof(buffer), "Server PID: %d\nCurrent Date and Time: %s", pid, time_str);

            // Step 11: Send the message to the client
            n = write(newsockfd, buffer, strlen(buffer));
            if (n < 0) {
                perror("Error writing to socket");
                exit(1);
            }

            // Step 12: Close the socket and terminate the child process
            close(newsockfd);
            exit(0);
        } else {
            // Parent process: Close the client socket and continue accepting new connections
            close(newsockfd);
        }
    }

    // Step 13: Close the server socket (unreachable in this example)
    close(sockfd);
    return 0;
}
Client Code
#include<sys/types.h>      // Data type definitions for sockets
#include<sys/socket.h>     // Socket functions
#include<stdio.h>          // Standard input/output functions
#include<netinet/in.h>     // Contains structures for internet addresses
#include<arpa/inet.h>      // For handling internet operations
#include<string.h>         // String handling functions
#include<unistd.h>         // POSIX API for close
#include<stdlib.h>         // For exit

#define PORTNO 10200       // Port number the server listens on
#define BUFFER_SIZE 1024   // Buffer size for receiving messages

int main() {
    int sockfd, n;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE];   // Buffer to receive the message from the server

    // Step 1: Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Step 2: Initialize the server address structure
    servaddr.sin_family = AF_INET;                      // Use IPv4 protocol
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Connect to localhost
    servaddr.sin_port = htons(PORTNO);                  // Convert port number to network byte order

    // Step 3: Connect to the server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Step 4: Read the message from the server
    n = read(sockfd, buffer, sizeof(buffer));
    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }
    buffer[n] = '\0';  // Null-terminate the received string

    // Step 5: Display the message from the server (current date, time, and PID)
    printf("Received from server:\n%s\n", buffer);

    // Step 6: Close the socket
    close(sockfd);
    return 0;
}
Explanation
Server:

The server listens for incoming client connections using TCP.
It forks a new process to handle each client connection, allowing concurrent connections.
The server sends the current date and time along with its process ID (PID) to the client.
  The child process handles communication with the client and terminates after 
the message is sent, while the parent process continues to listen for new connections.
Client:

The client connects to the server, receives the current date,
time, and PID from the server, and then displays the message on the screen.

gcc -o tcp_server tcp_server.c
gcc -o tcp_client tcp_client.c
./tcp_server
./tcp_client

The client will display the current date, time, and PID of the
server process that handled the connection.
 Both processes will terminate after processing the message.
