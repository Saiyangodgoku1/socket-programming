/*
Write a client program that manually crafts an HTTP request packet,
sends it to a web server, and displays all fields received in the HTTP response.
This should be implemented for Linux systems using sockets.*/
Client Code
#include<stdio.h>          // Standard I/O functions
#include<stdlib.h>         // Standard library functions
#include<string.h>         // String handling functions
#include<sys/socket.h>     // Socket functions
#include<arpa/inet.h>      // Contains structures for internet addresses
#include<netdb.h>          // For gethostbyname() to resolve domain names
#include<unistd.h>         // For POSIX API like close

#define PORTNO 80          // HTTP operates on port 80

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    struct hostent *server;
    char request[1024], response[4096];   // Buffers for HTTP request and response
    int n;

    // Step 1: Resolve the hostname to an IP address (e.g., google.com)
    char hostname[] = "www.example.com";  // Replace with the website you want to send the request to
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr, "Error: No such host\n");
        exit(1);
    }

    // Step 2: Create a socket using TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Step 3: Initialize the server address structure
    servaddr.sin_family = AF_INET;  // Use IPv4
    servaddr.sin_port = htons(PORTNO);  // Convert port number to network byte order
    memcpy(&servaddr.sin_addr.s_addr, server->h_addr, server->h_length);  // Copy server IP address

    // Step 4: Connect to the web server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Step 5: Craft a manual HTTP GET request
    snprintf(request, sizeof(request),
        "GET / HTTP/1.1\r\n"        // HTTP GET request for the root page
        "Host: %s\r\n"              // Host header with the server hostname
        "Connection: close\r\n\r\n",// Close the connection after the request
        hostname);                  // Inject the hostname into the request
    printf("HTTP request:\n%s\n", request);  // Print the HTTP request

    // Step 6: Send the crafted HTTP request to the server
    n = write(sockfd, request, strlen(request));
    if (n < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    // Step 7: Read and display the HTTP response
    printf("HTTP response:\n");
    while ((n = read(sockfd, response, sizeof(response) - 1)) > 0) {
        response[n] = '\0';  // Null-terminate the response string
        printf("%s", response);  // Print the response
    }
    if (n < 0) {
        perror("Error reading from socket");
    }

    // Step 8: Close the socket
    close(sockfd);
    return 0;
}
/*Explanation of the Code
Step 1: The program first resolves the domain name (e.g., www.example.com) to an IP address using gethostbyname().
Step 2: A TCP socket is created using socket().
Step 3: The server address structure (struct sockaddr_in) is initialized with the server's IP address and port 80 (HTTP).
Step 4: The program establishes a connection to the web server using connect().
Step 5: A manual HTTP GET request is crafted for the root page (/) of the web server, including the Host and Connection: close headers.
Step 6: The HTTP request is sent to the server using write().
Step 7: The HTTP response is read in chunks and displayed on the client side using read() and printed in a loop.
Step 8: The socket is closed using close().*/
gcc -o http_client http_client.c
./http_client
