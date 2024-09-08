/*
Write an iterative TCP client-server program where the client accepts a sentence
from the user and sends it to the server. The server will check for duplicate words in the string,
find the number of occurrences of duplicate words, and remove the duplicates by retaining
a single occurrence of each word. The server sends the resultant sentence back to the client. 
The client displays the received data. The process repeats until 
the client enters the string "Stop," 
at which point both processes terminate.*/
Server Code
#include<stdio.h>          // Standard I/O functions
#include<string.h>         // String handling functions
#include<sys/types.h>      // Definitions for data types like socket structures
#include<sys/socket.h>     // Contains socket functions
#include<netinet/in.h>     // Contains constants and structures for internet domain addresses
#include<arpa/inet.h>      // Contains definitions for internet operations
#include<unistd.h>         // POSIX API for sleep, close, etc.
#include<stdlib.h>         // For general utilities like exit
#define PORTNO 10200       // Server will listen on port 10200

// Helper function to remove duplicate words
void remove_duplicates(char *input, char *output) {
    char temp[256];
    char *word;
    int found;
    strcpy(temp, input);    // Copy input sentence into temporary buffer
    char *token[50];        // Token array to store words
    int count[50] = {0};    // Array to count occurrences of each word
    int i = 0, j;

    word = strtok(temp, " ");  // Tokenize the sentence by spaces
    while (word != NULL) {
        found = 0;
        // Check for duplicates
        for (j = 0; j < i; j++) {
            if (strcmp(token[j], word) == 0) {
                count[j]++;   // Increment the count for duplicates
                found = 1;
                break;
            }
        }
        // If the word is not a duplicate, store it
        if (!found) {
            token[i] = word;
            count[i] = 1;
            i++;
        }
        word = strtok(NULL, " "); // Get the next word
    }

    // Construct the resultant sentence
    output[0] = '\0';  // Clear output buffer
    for (j = 0; j < i; j++) {
        strcat(output, token[j]);
        strcat(output, " ");
    }
    output[strlen(output) - 1] = '\0';  // Remove the trailing space
}

int main() {
    int sockfd, newsockfd, clilen, n = 1;
    struct sockaddr_in seraddr, cliaddr;
    char buf[256], result[256];

    // Step 1: Create socket using TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        return 1;
    }

    // Step 2: Initialize server address structure
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("172.16.48.92");
    seraddr.sin_port = htons(PORTNO);

    // Step 3: Bind the socket to the specified port
    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Error binding socket");
        return 1;
    }

    // Step 4: Listen for incoming client connections
    listen(sockfd, 5);
    printf("Server waiting for client...\n");

    while (1) {
        // Step 5: Accept client connection
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (newsockfd < 0) {
            perror("Error on accept");
            return 1;
        }

        // Step 6: Read message from client
        n = read(newsockfd, buf, sizeof(buf));
        if (n < 0) {
            perror("Error reading from socket");
            return 1;
        }
        buf[n] = '\0'; // Null-terminate the string
        printf("Message from Client: %s\n", buf);

        // If the client sends "Stop", terminate the server
        if (strcmp(buf, "Stop") == 0) {
            printf("Termination requested by client. Stopping server...\n");
            break;
        }

        // Step 7: Remove duplicates and prepare result
        remove_duplicates(buf, result);
        printf("Processed sentence: %s\n", result);

        // Step 8: Send the processed result back to the client
        n = write(newsockfd, result, strlen(result) + 1);
        if (n < 0) {
            perror("Error writing to socket");
            return 1;
        }
    }

    // Step 9: Close server socket
    close(newsockfd);
    close(sockfd);
    return 0;
}
Client Code 
#include<sys/types.h>      // Definitions for data types like socket structures
#include<sys/socket.h>     // Contains socket functions
#include<stdio.h>          // Standard I/O functions
#include<netinet/in.h>     // Contains constants and structures for internet domain addresses
#include<arpa/inet.h>      // Contains definitions for internet operations
#include<string.h>         // String handling functions
#include<unistd.h>         // POSIX API for sleep, close, etc.
#include<stdlib.h>         // For general utilities like exit

int main() {
    int len, result, sockfd, n = 1;
    struct sockaddr_in address;
    char ch[256], buf[256];

    // Step 1: Create a socket using TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        return 1;
    }

    // Step 2: Initialize server address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("172.16.48.92");
    address.sin_port = htons(10200);
    len = sizeof(address);

    // Step 3: Connect to the server
    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1) {
        perror("Error connecting to server");
        exit(1);
    }

    // Step 4: Keep interacting with the server until "Stop" is entered
    while (1) {
        // Get the sentence from the user
        printf("Enter a string (or 'Stop' to exit): ");
        fgets(ch, sizeof(ch), stdin);
        ch[strlen(ch) - 1] = '\0';  // Remove trailing newline

        // Step 5: Send the message to the server
        write(sockfd, ch, strlen(ch) + 1);

        // Step 6: If the user enters "Stop", terminate the client
        if (strcmp(ch, "Stop") == 0) {
            printf("Terminating client...\n");
            break;
        }

        // Step 7: Read the processed message from the server
        n = read(sockfd, buf, sizeof(buf));
        if (n < 0) {
            perror("Error reading from socket");
            return 1;
        }
        buf[n] = '\0';  // Null-terminate the received string

        // Step 8: Display the result sent by the server
        printf("Processed sentence from server: %s\n", buf);
    }

    // Step 9: Close the socket
    close(sockfd);
    return 0;
}
