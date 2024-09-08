/*
Write a TCP client-server program to illustrate encryption and decryption of messages.
The client accepts a message, encrypts it by adding 4 to the ASCII value of each alphabet,
and sends the encrypted message to the server. The server decrypts the message by reversing 
the encryption and displays both the encrypted and decrypted forms of the string.
The program terminates after one session.*/
Server Code
#include<stdio.h>          // Standard I/O functions
#include<string.h>         // String handling functions
#include<sys/types.h>      // Data type definitions for sockets
#include<sys/socket.h>     // Socket functions
#include<netinet/in.h>     // Contains structures for internet addresses
#include<arpa/inet.h>      // For handling internet operations
#include<unistd.h>         // POSIX API for close, sleep, etc.

#define PORTNO 10200       // Port number the server will listen on

// Helper function to decrypt the message by subtracting 4 from the ASCII value of each alphabet
void decrypt_message(char *encrypted_message, char *decrypted_message) {
    int i = 0;
    while (encrypted_message[i] != '\0') {
        // Only decrypt alphabetic characters
        if ((encrypted_message[i] >= 'A' && encrypted_message[i] <= 'Z') || (encrypted_message[i] >= 'a' && encrypted_message[i] <= 'z')) {
            decrypted_message[i] = encrypted_message[i] - 4; // Reverse encryption by subtracting 4
        } else {
            decrypted_message[i] = encrypted_message[i]; // Non-alphabetic characters remain unchanged
        }
        i++;
    }
    decrypted_message[i] = '\0';  // Null-terminate the decrypted message
}

int main() {
    int sockfd, newsockfd, clilen, n;
    struct sockaddr_in seraddr, cliaddr;
    char encrypted_message[256], decrypted_message[256];

    // Step 1: Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        return 1;
    }

    // Step 2: Initialize the server address structure
    seraddr.sin_family = AF_INET;                    // Use the Internet domain (IPv4)
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Bind to localhost IP address
    seraddr.sin_port = htons(PORTNO);                // Convert port number to network byte order

    // Step 3: Bind the socket to the server address and port
    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Error binding socket");
        return 1;
    }

    // Step 4: Listen for incoming connections from clients
    listen(sockfd, 5);
    printf("Server waiting for client...\n");

    // Step 5: Accept client connection
    clilen = sizeof(cliaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
    if (newsockfd < 0) {
        perror("Error on accept");
        return 1;
    }

    // Step 6: Receive the encrypted message from the client
    n = read(newsockfd, encrypted_message, sizeof(encrypted_message));
    if (n < 0) {
        perror("Error reading from socket");
        return 1;
    }
    encrypted_message[n] = '\0'; // Null-terminate the received message
    printf("Encrypted message received from client: %s\n", encrypted_message);

    // Step 7: Decrypt the message
    decrypt_message(encrypted_message, decrypted_message);
    printf("Decrypted message: %s\n", decrypted_message);

    // Step 8: Close the sockets
    close(newsockfd);
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
#include<unistd.h>         // POSIX API for close, sleep, etc.

#define PORTNO 10200       // The port number to connect to the server

// Helper function to encrypt the message by adding 4 to the ASCII value of each alphabet
void encrypt_message(char *message, char *encrypted_message) {
    int i = 0;
    while (message[i] != '\0') {
        // Only encrypt alphabetic characters
        if ((message[i] >= 'A' && message[i] <= 'Z') || (message[i] >= 'a' && message[i] <= 'z')) {
            encrypted_message[i] = message[i] + 4; // Encrypt by adding 4
        } else {
            encrypted_message[i] = message[i]; // Non-alphabetic characters remain unchanged
        }
        i++;
    }
    encrypted_message[i] = '\0'; // Null-terminate the encrypted message
}

int main() {
    int sockfd, n;
    struct sockaddr_in address;
    char message[256], encrypted_message[256];

    // Step 1: Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        return 1;
    }

    // Step 2: Initialize the server address structure
    address.sin_family = AF_INET;                   // Use the Internet domain (IPv4)
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Connect to localhost server
    address.sin_port = htons(PORTNO);               // Convert port number to network byte order

    // Step 3: Connect to the server
    if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error connecting to server");
        return 1;
    }

    // Step 4: Get the message from the user
    printf("Enter a message to encrypt: ");
    fgets(message, sizeof(message), stdin);
    message[strlen(message) - 1] = '\0'; // Remove the trailing newline character

    // Step 5: Encrypt the message
    encrypt_message(message, encrypted_message);
    printf("Encrypted message: %s\n", encrypted_message);

    // Step 6: Send the encrypted message to the server
    n = write(sockfd, encrypted_message, strlen(encrypted_message) + 1);
    if (n < 0) {
        perror("Error writing to socket");
        return 1;
    }

    // Step 7: Close the socket
    close(sockfd);
    return 0;
}
Explanation of the Code

Server: The server listens for a connection from the client. Once a connection is established, 
it receives the encrypted message, decrypts it by subtracting 4 from 
the ASCII value of each alphabetic character,
and displays both the encrypted and decrypted messages. 
After processing one message, the server terminates.

Client: The client takes input from the user, encrypts the message by adding 4 to the ASCII 
value of each alphabetic character, and sends the encrypted message to the server.
  After sending the message, the client terminates.

gcc -o tcp_server tcp_server.c
gcc -o tcp_client tcp_client.c
./tcp_server
./tcp_client
The client will prompt you to enter a message, which it will encrypt and send to the server.
The server will display both the encrypted message and the decrypted message.
