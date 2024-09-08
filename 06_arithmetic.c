/*
Implement a concurrent Remote Math Server that performs arithmetic operations on the server side. 
The client accepts two integers and an operator from the user and sends them to the server.
The server receives the integers and operator, performs the requested operation (addition, subtraction, multiplication, or division), 
and sends the result back to the client. Both the client and server terminate after the result is displayed.*/

Server Code:

#include<stdio.h>          // Standard input/output functions
#include<string.h>         // String handling functions
#include<stdlib.h>         // Standard library functions
#include<sys/socket.h>     // Socket functions
#include<netinet/in.h>     // Contains structures for internet addresses
#include<unistd.h>         // For POSIX API like close, fork
#include<arpa/inet.h>      // For handling internet operations
#include<signal.h>         // For signal handling to manage child processes

#define PORTNO 10200       // Port number the server will listen on

// Function to handle the arithmetic operation
float perform_operation(int num1, int num2, char operator) {
    switch (operator) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return (num2 != 0) ? (float)num1 / num2 : 0; // Prevent division by zero
        default: return 0;  // If an invalid operator is received
    }
}

// Signal handler to reap zombie child processes
void handle_sigchld(int sig) {
    // Reap child processes
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    int sockfd, newsockfd, clilen, n;
    struct sockaddr_in seraddr, cliaddr;
    char operator;
    int num1, num2;
    float result;

    // Step 1: Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Step 2: Initialize the server address structure
    seraddr.sin_family = AF_INET;                    // Use IPv4 protocol
    seraddr.sin_addr.s_addr = INADDR_ANY;            // Bind to all available interfaces
    seraddr.sin_port = htons(PORTNO);                // Convert port number to network byte order

    // Step 3: Bind the socket to the server's address and port
    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    // Step 4: Listen for incoming client connections
    listen(sockfd, 5);
    printf("Server waiting for client...\n");

    // Step 5: Set up signal handler to prevent zombie processes
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

            // Step 8: Read the two integers and operator from the client
            n = read(newsockfd, &num1, sizeof(int));
            if (n < 0) {
                perror("Error reading num1");
                exit(1);
            }

            n = read(newsockfd, &num2, sizeof(int));
            if (n < 0) {
                perror("Error reading num2");
                exit(1);
            }

            n = read(newsockfd, &operator, sizeof(char));
            if (n < 0) {
                perror("Error reading operator");
                exit(1);
            }

            // Step 9: Perform the requested arithmetic operation
            result = perform_operation(num1, num2, operator);
            printf("Operation: %d %c %d = %f\n", num1, operator, num2, result);

            // Step 10: Send the result back to the client
            n = write(newsockfd, &result, sizeof(result));
            if (n < 0) {
                perror("Error writing to socket");
                exit(1);
            }

            // Step 11: Close the socket and terminate the child process
            close(newsockfd);
            exit(0);
        } else {
            // Parent process: Close the client socket and continue accepting new connections
            close(newsockfd);
        }
    }

    // Step 12: Close the server socket (unreachable in this example)
    close(sockfd);
    return 0;
}

Client Code
#include<sys/types.h>      // Data type definitions for sockets
#include<sys/socket.h>     // Socket functions
#include<stdio.h>          // Standard I/O functions
#include<netinet/in.h>     // Contains structures for internet addresses
#include<arpa/inet.h>      // For handling internet operations
#include<string.h>         // String handling functions
#include<unistd.h>         // For POSIX API like close
#include<stdlib.h>         // For exit

#define PORTNO 10200       // Port number the server listens on

int main() {
    int sockfd, n;
    struct sockaddr_in servaddr;
    int num1, num2;
    char operator;
    float result;

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

    // Step 4: Accept input from the user (two integers and an operator)
    printf("Enter first integer: ");
    scanf("%d", &num1);

    printf("Enter second integer: ");
    scanf("%d", &num2);

    printf("Enter an operator (+, -, *, /): ");
    scanf(" %c", &operator);  // Space before %c to avoid reading newline character

    // Step 5: Send the two integers and the operator to the server
    n = write(sockfd, &num1, sizeof(int));
    if (n < 0) {
        perror("Error writing num1 to socket");
        exit(1);
    }

    n = write(sockfd, &num2, sizeof(int));
    if (n < 0) {
        perror("Error writing num2 to socket");
        exit(1);
    }

    n = write(sockfd, &operator, sizeof(char));
    if (n < 0) {
        perror("Error writing operator to socket");
        exit(1);
    }

    // Step 6: Read the result sent by the server
    n = read(sockfd, &result, sizeof(float));
    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }

    // Step 7: Display the result
    printf("Result of %d %c %d = %f\n", num1, operator, num2, result);

    // Step 8: Close the socket
    close(sockfd);
    return 0;
}

Explanation
Server:

The server is a concurrent server that can handle 
multiple client connections simultaneously using fork().
The server receives two integers and an operator from the client,
performs the arithmetic operation, and sends the result back to the client.
After handling a client, the child process terminates,
while the parent process continues to listen for new clients.

Client:

The client takes two integers and an operator from the user, 
sends them to the server, and receives the result of the operation.

gcc -o math_server math_server.c
gcc -o math_client math_client.c

./math_server
./math_client

  The client will prompt you to enter two integers and an operator. 
It will then display the result of the arithmetic operation after receiving it from the server. 
  Both processes will terminate after the result is shown.
