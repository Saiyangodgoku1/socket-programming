/*
Write a concurrent TCP daytime server program in C where the server sends the current date 
and time along with its process ID (PID) to the client. 
The server should be able to handle multiple clients concurrently using the fork() system call.*/

Server Code
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>

#define PORT 9090

// Function to get the current date and time
void get_current_time(char *buffer) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", tm_info);
}

int main() {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024], response[1024];
    pid_t pid;

    // Step 1: Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Setup server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Step 3: Listen for client connections
    listen(sockfd, 5);

    while (1) {
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);

        if (fork() == 0) {
            // Step 4: Get current time
            get_current_time(buffer);

            // Step 5: Get the process ID (PID)
            pid = getpid();

            // Step 6: Prepare response with time and PID
            snprintf(response, sizeof(response), "Current Time: %s, Server PID: %d", buffer, pid);

            // Step 7: Send response to client
            write(newsockfd, response, strlen(response) + 1);

            // Step 8: Close client socket
            close(newsockfd);
            exit(0);
        }

        close(newsockfd);
    }

    // Step 9: Close server socket
    close(sockfd);
    return 0;
}

Client Code
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 9090

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];

    // Step 1: Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Setup server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Step 3: Connect to server
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Step 4: Receive and display server's response with time and PID
    read(sockfd, buffer, sizeof(buffer));
    printf("Server Response: %s\n", buffer);

    // Step 5: Close socket
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
