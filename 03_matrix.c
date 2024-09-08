/*
Write an iterative UDP client-server program where the client sends rows of a matrix to the server,
and the server combines them together into a complete matrix. 
The server sends a confirmation back for each row received. The client can send multiple rows,
and the process repeats until the matrix is complete.*/
Server Code 
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#define PORT 8080
#define MAX_ROWS 10
#define MAX_COLS 10

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    int matrix[MAX_ROWS][MAX_COLS], row[MAX_COLS], row_count = 0;
    socklen_t len;
    
    // Step 1: Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Step 2: Setup server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    
    // Step 3: Bind socket to address
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    len = sizeof(cliaddr);
    
    // Step 4: Receive rows of the matrix from the client
    while (row_count < MAX_ROWS) {
        recvfrom(sockfd, row, sizeof(row), 0, (struct sockaddr *)&cliaddr, &len);
        for (int i = 0; i < MAX_COLS; i++) {
            matrix[row_count][i] = row[i];  // Store received row
        }
        row_count++;
    }

    // Step 5: Close socket after receiving the entire matrix
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
#define MAX_ROWS 10
#define MAX_COLS 10

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    int matrix[MAX_ROWS][MAX_COLS], row[MAX_COLS];
    socklen_t len;

    // Step 1: Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Step 2: Setup server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    len = sizeof(servaddr);

    // Step 3: Input matrix from user
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            scanf("%d", &matrix[i][j]);  // Input matrix row by row
        }
    }

    // Step 4: Send each row of the matrix to the server
    for (int i = 0; i < MAX_ROWS; i++) {
        memcpy(row, matrix[i], sizeof(row));  // Copy row to send
        sendto(sockfd, row, sizeof(row), 0, (struct sockaddr *)&servaddr, len);
    }

    // Step 5: Close socket after sending all rows
    close(sockfd);
    return 0;
}

gcc -o udp_server udp_server.c
gcc -o udp_client udp_client.c
one terminal:
./udp_server
another terminal:
./udp_client
