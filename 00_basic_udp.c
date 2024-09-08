/*
Explanation of the Code Structure
UDP:

Server:
Creates a UDP socket using socket().
Binds the socket to a specific port.
Waits to receive data from the client using recvfrom().
Processes the data and sends a response using sendto().
Client:
Creates a UDP socket.
Sends data to the server using sendto().
Receives the server's response using recvfrom().
Closes the socket.
*/
UDP Server (General Syntax)
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
    struct sockaddr_in seraddr, cliaddr;
    char buffer[1024];
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = INADDR_ANY;
    seraddr.sin_port = htons(PORTNO);
    bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr));

    len = sizeof(cliaddr);
    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&cliaddr, &len);
    sendto(sockfd, "Received", 8, 0, (struct sockaddr *)&cliaddr, len);

    close(sockfd);
    return 0;
}

UDP Client (General Syntax)
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
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORTNO);

    len = sizeof(servaddr);
    sendto(sockfd, "Hello Server", 12, 0, (struct sockaddr *)&servaddr, len);
    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&servaddr, &len);

    close(sockfd);
    return 0;
}

