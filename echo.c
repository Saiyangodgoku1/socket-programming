/*
You are required to create a TCP server and client where the server listens for incoming connections
from the client. The client sends a message to the server, and the server responds by sending the same
message back to the client. The server operates continuously, handling multiple client connections,
while the client sends one message and receives a response before terminating.
*/
Server Code 

#include<stdio.h>          // Standard input/output functions
#include<string.h>         // String handling functions
#include<sys/types.h>      // Definitions for data types like socket structures
#include<sys/socket.h>     // Contains socket functions
#include<netinet/in.h>     // Contains constants and structures for internet domain addresses
#include<arpa/inet.h>      // Definitions for internet operations
#include<unistd.h>         // Contains constants and types for POSIX systems

#define PORTNO 10200       // Defining the port number the server will listen on

int main() {
    int sockfd, newsockfd, clilen, n = 1;       // Variables for socket file descriptors, client length, and data size
    struct sockaddr_in seraddr, cliaddr;        // Structures for server and client addresses
    char buf[256];                              // Buffer to store messages

    // Step 1: Creating the server socket using TCP (SOCK_STREAM)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");         // Error handling in case socket creation fails
        return 1;                               // Return non-zero to indicate failure
    }

    // Step 2: Initialize server address structure
    seraddr.sin_family = AF_INET;               // Using the Internet domain (IPv4)
    seraddr.sin_addr.s_addr = inet_addr("172.16.48.92");  // Server IP address (local machine or specific IP)
    seraddr.sin_port = htons(PORTNO);           // Convert port number to network byte order (host to network short)

    // Step 3: Bind the server socket to the specified address and port
    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Error binding socket");         // Error handling for binding failure
        return 1;                               // Return non-zero to indicate failure
    }

    // Step 4: Listen for incoming connections, with a maximum queue of 5 clients
    listen(sockfd, 5);
    printf("Server waiting for client...\n");   // Inform that server is waiting for client connection

    // Step 5: Server operates in an infinite loop to handle multiple clients
    while (1) {
        clilen = sizeof(cliaddr);               // Get the size of the client address structure

        // Step 6: Accept the client connection
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (newsockfd < 0) {
            perror("Error on accept");          // Error handling if accepting connection fails
            return 1;                           // Return non-zero to indicate failure
        }

        // Step 7: Read message from client
        n = read(newsockfd, buf, sizeof(buf));
        if (n < 0) {
            perror("Error reading from socket");  // Error handling for reading failure
            return 1;                             // Return non-zero to indicate failure
        }
        printf("Message from Client: %s\n", buf); // Print received message

        // Step 8: Write the received message back to the client (echoing)
        n = write(newsockfd, buf, sizeof(buf));
        if (n < 0) {
            perror("Error writing to socket");    // Error handling for writing failure
            return 1;                             // Return non-zero to indicate failure
        }
    }

    // Close the socket (unreachable in this code since the server runs infinitely)
    return 0;
}

Question
You are required to create a TCP server and client where the server listens for incoming connections from the client. The client sends a message to the server, and the server responds by sending the same message back to the client. The server operates continuously, handling multiple client connections, while the client sends one message and receives a response before terminating.

Server Code (Heavily Commented)
c
Copy code
#include<stdio.h>          // Standard input/output functions
#include<string.h>         // String handling functions
#include<sys/types.h>      // Definitions for data types like socket structures
#include<sys/socket.h>     // Contains socket functions
#include<netinet/in.h>     // Contains constants and structures for internet domain addresses
#include<arpa/inet.h>      // Definitions for internet operations
#include<unistd.h>         // Contains constants and types for POSIX systems

#define PORTNO 10200       // Defining the port number the server will listen on

int main() {
    int sockfd, newsockfd, clilen, n = 1;       // Variables for socket file descriptors, client length, and data size
    struct sockaddr_in seraddr, cliaddr;        // Structures for server and client addresses
    char buf[256];                              // Buffer to store messages

    // Step 1: Creating the server socket using TCP (SOCK_STREAM)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");         // Error handling in case socket creation fails
        return 1;                               // Return non-zero to indicate failure
    }

    // Step 2: Initialize server address structure
    seraddr.sin_family = AF_INET;               // Using the Internet domain (IPv4)
    seraddr.sin_addr.s_addr = inet_addr("172.16.48.92");  // Server IP address (local machine or specific IP)
    seraddr.sin_port = htons(PORTNO);           // Convert port number to network byte order (host to network short)

    // Step 3: Bind the server socket to the specified address and port
    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Error binding socket");         // Error handling for binding failure
        return 1;                               // Return non-zero to indicate failure
    }

    // Step 4: Listen for incoming connections, with a maximum queue of 5 clients
    listen(sockfd, 5);
    printf("Server waiting for client...\n");   // Inform that server is waiting for client connection

    // Step 5: Server operates in an infinite loop to handle multiple clients
    while (1) {
        clilen = sizeof(cliaddr);               // Get the size of the client address structure

        // Step 6: Accept the client connection
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (newsockfd < 0) {
            perror("Error on accept");          // Error handling if accepting connection fails
            return 1;                           // Return non-zero to indicate failure
        }

        // Step 7: Read message from client
        n = read(newsockfd, buf, sizeof(buf));
        if (n < 0) {
            perror("Error reading from socket");  // Error handling for reading failure
            return 1;                             // Return non-zero to indicate failure
        }
        printf("Message from Client: %s\n", buf); // Print received message

        // Step 8: Write the received message back to the client (echoing)
        n = write(newsockfd, buf, sizeof(buf));
        if (n < 0) {
            perror("Error writing to socket");    // Error handling for writing failure
            return 1;                             // Return non-zero to indicate failure
        }
    }

    // Close the socket (unreachable in this code since the server runs infinitely)
    return 0;
}
Client Code
#include<sys/types.h>      // Definitions for data types used in socket structures
#include<sys/socket.h>     // Contains socket functions
#include<stdio.h>          // Standard input/output functions
#include<netinet/in.h>     // Contains constants and structures for internet domain addresses
#include<arpa/inet.h>      // Definitions for internet operations
#include<stdlib.h>         // General utilities, including memory allocation
#include<string.h>         // String handling functions
#include<unistd.h>         // Contains constants and types for POSIX systems

int main() {
    int len, result, sockfd, n = 1;             // Variables for socket file descriptor, connection result, and data size
    struct sockaddr_in address;                 // Structure for the client address
    char ch[256], buf[256];                     // Buffers for sending and receiving messages

    // Step 1: Create a socket for the client using TCP (SOCK_STREAM)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");         // Error handling if socket creation fails
        return 1;                              // Return non-zero to indicate failure
    }

    // Step 2: Initialize the client address structure
    address.sin_family = AF_INET;              // Using the Internet domain (IPv4)
    address.sin_addr.s_addr = inet_addr("172.16.48.92");  // Server IP address to connect to
    address.sin_port = htons(10200);           // Convert port number to network byte order (host to network short)
    len = sizeof(address);                     // Get the size of the address structure

    // Step 3: Connect to the server
    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1) {
        perror("CLIENT ERROR");                // Error handling if connection to the server fails
        exit(1);                               // Exit the program with failure
    }

    // Step 4: Get a message from the user to send to the server
    printf("Enter a string: ");
    fgets(ch, sizeof(ch), stdin);              // Read input from the user
    ch[strlen(ch) - 1] = '\0';                 // Remove the newline character from input

    // Step 5: Send the message to the server
    write(sockfd, ch, strlen(ch));

    // Step 6: Read the echoed message from the server
    n = read(sockfd, buf, sizeof(buf));
    buf[n] = '\0';                             // Ensure the buffer is null-terminated for printing

    // Step 7: Display the server's response (echoed message)
    printf("String sent back from server: %s\n", buf);

    // Step 8: Close the socket connection
    close(sockfd);
    return 0;
}
/*These codes create a simple TCP echo server and client.
The server continuously listens for client connections, reads a message,
and echoes it back to the client. 
The client sends a single message to the server and prints the echoed response.*/
