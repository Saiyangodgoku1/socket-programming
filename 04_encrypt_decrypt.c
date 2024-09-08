/*
Write a TCP client-server program to illustrate encryption and decryption of messages.
The client accepts a message, encrypts it by adding 4 to the ASCII value of each alphabet,
and sends the encrypted message to the server. The server decrypts the message by reversing 
the encryption and displays both the encrypted and decrypted forms of the string.
The program terminates after one session.*/
Server Code
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#define PORT 8080

// Decrypt the message by subtracting 4 from ASCII value
void decrypt_message(char *encrypted, char *decrypted) {
    for (int i = 0; encrypted[i] != '\0'; i++) {
        if ((encrypted[i] >= 'A' && encrypted[i] <= 'Z') || (encrypted[i] >= 'a' && encrypted[i] <= 'z')) {
            decrypted[i] = encrypted[i] - 4;
        } else {
            decrypted[i] = encrypted[i];  // Non-alphabetic characters remain unchanged
        }
    }
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in servaddr, cliaddr;
    char encrypted[1024], decrypted[1024];
    socklen_t clilen;

    // Step 1: Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Setup server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);

    // Step 3: Accept connection from client
    clilen = sizeof(cliaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);

    // Step 4: Receive encrypted message from client
    read(newsockfd, encrypted, sizeof(encrypted));

    // Step 5: Decrypt the message
    decrypt_message(encrypted, decrypted);

    // Step 6: Close socket
    close(newsockfd);
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

// Encrypt the message by adding 4 to ASCII value
void encrypt_message(char *message, char *encrypted) {
    for (int i = 0; message[i] != '\0'; i++) {
        if ((message[i] >= 'A' && message[i] <= 'Z') || (message[i] >= 'a' && message[i] <= 'z')) {
            encrypted[i] = message[i] + 4;
        } else {
            encrypted[i] = message[i];  // Non-alphabetic characters remain unchanged
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char message[1024], encrypted[1024];

    // Step 1: Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Setup server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Step 3: Connect to server
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Step 4: Accept message from user and encrypt it
    fgets(message, sizeof(message), stdin);
    encrypt_message(message, encrypted);

    // Step 5: Send encrypted message to server
    write(sockfd, encrypted, strlen(encrypted) + 1);

    // Step 6: Close socket
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
