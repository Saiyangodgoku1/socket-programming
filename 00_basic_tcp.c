/*Explanation of the Code Structure
TCP:

Server:
Creates a TCP socket using socket().
Binds the socket to a specific port using bind().
Listens for incoming connections using listen().
Accepts a connection from a client using accept().
Reads data from the client, processes it, and sends a response.
Client:
Creates a TCP socket.
Connects to the server using connect().*/
TCP Server (General Syntax)

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORTNO 8080

int main() {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in seraddr, cliaddr;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = INADDR_ANY;
    seraddr.sin_port = htons(PORTNO);
    bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
    listen(sockfd, 5);
    
    clilen = sizeof(cliaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
    read(newsockfd, buffer, 1024);
    write(newsockfd, "Received", 8);
    
    close(newsockfd);
    close(sockfd);
    return 0;
}

TCP Client (General Syntax)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORTNO 8080

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORTNO);

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    write(sockfd, "Hello Server", 12);
    read(sockfd, buffer, 1024);
    
    close(sockfd);
    return 0;
}


Sends data to the server, receives a response, and closes the connection.*/
