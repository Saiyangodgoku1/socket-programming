/*
Implement a concurrent Remote Math Server that performs arithmetic operations on the server side. 
The client accepts two integers and an operator from the user and sends them to the server.
The server receives the integers and operator, performs the requested operation (addition, subtraction, multiplication, or division), 
and sends the result back to the client. Both the client and server terminate after the result is displayed.*/

Server Code:
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>

#define PORT 8080

// Function to perform arithmetic operation
float perform_operation(int num1, int num2, char operator) {
    switch (operator) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return (num2 != 0) ? (float)num1 / num2 : 0;
        default: return 0;
    }
}

int main() {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in servaddr, cliaddr;
    int num1, num2;
    char operator;
    float result;

    // Step 1: Create a TCP socket
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
            // Step 4: Receive integers and operator from client
            read(newsockfd, &num1, sizeof(int));
            read(newsockfd, &num2, sizeof(int));
            read(newsockfd, &operator, sizeof(char));

            // Step 5: Perform the operation and send result to client
            result = perform_operation(num1, num2, operator);
            write(newsockfd, &result, sizeof(result));

            // Step 6: Close client socket
            close(newsockfd);
            exit(0);
        }
        close(newsockfd);
    }

    // Step 7: Close server socket (never reached)
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

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    int num1, num2;
    char operator;
    float result;

    // Step 1: Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Setup server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Step 3: Connect to the server
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Step 4: Accept two integers and operator from the user
    printf("Enter first integer: ");
    scanf("%d", &num1);
    printf("Enter second integer: ");
    scanf("%d", &num2);
    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &operator);  // Space before %c to avoid reading a newline character

    // Step 5: Send integers and operator to the server
    write(sockfd, &num1, sizeof(int));
    write(sockfd, &num2, sizeof(int));
    write(sockfd, &operator, sizeof(char));

    // Step 6: Receive result from the server
    read(sockfd, &result, sizeof(result));
    printf("Result: %f\n", result);

    // Step 7: Close the socket
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
