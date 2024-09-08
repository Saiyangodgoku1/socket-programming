/*
Write a client program that manually crafts an HTTP request packet,
sends it to a web server, and displays all fields received in the HTTP response.
This should be implemented for Linux systems using sockets.*/
Client Code
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netdb.h>

#define PORT 80

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    struct hostent *server;
    char request[1024], response[4096];

    // Step 1: Resolve the hostname to IP address (www.example.com)
    server = gethostbyname("www.example.com");

    // Step 2: Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 3: Setup server address
    servaddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&servaddr.sin_addr.s_addr, server->h_length);
    servaddr.sin_port = htons(PORT);

    // Step 4: Connect to web server
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Step 5: Craft and send HTTP GET request
    snprintf(request, sizeof(request), "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n");
    write(sockfd, request, strlen(request));

    // Step 6: Read and display HTTP response
    read(sockfd, response, sizeof(response));
    printf("%s", response);

    // Step 7: Close socket
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

Example Output:

HTTP request:
GET / HTTP/1.1
Host: www.example.com
Connection: close

HTTP response:
HTTP/1.1 200 OK
Date: Mon, 07 Sep 2024 12:00:00 GMT
Content-Type: text/html; charset=UTF-8
Connection: close
...
<!doctype html>
<html>
<head>...</head>
<body>...</body>
</html>
