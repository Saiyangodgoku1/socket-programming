/*
Write a concurrent TCP client server ‘C’ program where the client accepts a sentence from the user
and sends it to the server. The server will check for duplicate words in the string. Server will find the
number of occurrences of duplicate words present and remove the duplicate words by retaining single
occurrence of the word and send the resultant sentence to the client. 
*/
Server
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>

#define PORT 8080

// Function to remove duplicate words from the sentence
void remove_duplicates(char *input, char *output) {
    char *word;
    char *words[100];  // Array to store unique words
    int count[100] = {0}, num_words = 0;

    word = strtok(input, " ");
    while (word != NULL) {
        int found = 0;
        for (int i = 0; i < num_words; i++) {
            if (strcmp(words[i], word) == 0) {
                count[i]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            words[num_words] = word;
            count[num_words] = 1;
            num_words++;
        }
        word = strtok(NULL, " ");
    }

    output[0] = '\0';  // Clear the output buffer
    for (int i = 0; i < num_words; i++) {
        strcat(output, words[i]);
        strcat(output, " ");
    }
    output[strlen(output) - 1] = '\0';  // Remove the trailing space
}

int main() {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024], result[1024];

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
            // Step 4: Receive sentence from client
            read(newsockfd, buffer, sizeof(buffer));

            // Step 5: Remove duplicate words
            remove_duplicates(buffer, result);

            // Step 6: Send processed result back to client
            write(newsockfd, result, strlen(result) + 1);

            // Step 7: Close client socket
            close(newsockfd);
            exit(0);
        }

        close(newsockfd);
    }

    // Step 8: Close server socket
    close(sockfd);
    return 0;
}
client
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
    char buffer[1024];

    // Step 1: Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Setup server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Step 3: Connect to server
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Step 4: Accept sentence from user
    printf("Enter a sentence: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strlen(buffer) - 1] = '\0';  // Remove trailing newline

    // Step 5: Send sentence to server
    write(sockfd, buffer, strlen(buffer) + 1);

    // Step 6: Receive and display processed sentence from server
    read(sockfd, buffer, sizeof(buffer));
    printf("Processed Sentence: %s\n", buffer);

    // Step 7: Close socket
    close(sockfd);
    return 0;
}
Explanation (Heavily Commented)
1. Concurrent TCP Client-Server Program to Handle Duplicate Words:
Server: The server listens for incoming connections. When a client connects, the server forks a new process to handle the request. The server receives a sentence from the client, removes duplicate words, and sends the processed sentence back to the client.
Client: The client accepts a sentence from the user, sends it to the server, and displays the processed sentence after receiving it from the server.
