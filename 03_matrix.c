/*
Write an iterative UDP client-server program where the client sends rows of a matrix to the server,
and the server combines them together into a complete matrix. 
The server sends a confirmation back for each row received. The client can send multiple rows,
and the process repeats until the matrix is complete.*/
Server Code 
#include<stdio.h>          // Standard input/output functions
#include<string.h>         // String handling functions
#include<stdlib.h>         // General utility functions like malloc
#include<sys/socket.h>     // Contains socket functions
#include<netinet/in.h>     // Contains constants and structures for internet domain addresses
#include<arpa/inet.h>      // For handling internet operations
#include<unistd.h>         // Contains POSIX API like close, sleep, etc.

#define PORTNO 10200       // The port number on which the server will listen
#define MAX_ROWS 10        // Maximum number of rows in the matrix
#define MAX_COLS 10        // Maximum number of columns in the matrix

int main() {
    int sockfd, n;
    struct sockaddr_in seraddr, cliaddr;     // Structures for server and client addresses
    socklen_t len;                           // Length of client address
    int matrix[MAX_ROWS][MAX_COLS];          // Matrix to hold the combined result
    int row[MAX_COLS];                       // Buffer to hold the incoming row
    char buffer[256];                        // General-purpose buffer
    int row_count = 0, col_count = 0;        // Track number of rows and columns

    // Step 1: Create the UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        return 1;
    }

    // Step 2: Initialize server address structure
    seraddr.sin_family = AF_INET;            // Use IPv4 protocol
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost IP
    seraddr.sin_port = htons(PORTNO);        // Convert port number to network byte order

    // Step 3: Bind the server socket to the specified address and port
    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Error binding socket");
        return 1;
    }

    printf("Server ready to receive matrix rows...\n");

    // Step 4: Receive matrix rows from the client in a loop
    while (1) {
        len = sizeof(cliaddr);               // Set the size of the client address

        // Step 5: Receive a row from the client
        n = recvfrom(sockfd, row, sizeof(row), 0, (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            perror("Error receiving from client");
            return 1;
        }

        // Print received row for confirmation
        printf("Received row %d: ", row_count + 1);
        for (int i = 0; i < col_count; i++) {
            printf("%d ", row[i]);
            matrix[row_count][i] = row[i];   // Store the received row in the matrix
        }
        printf("\n");
        row_count++;  // Increment the row count

        // Step 6: Send confirmation back to the client
        snprintf(buffer, sizeof(buffer), "Row %d received", row_count);
        n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&cliaddr, len);
        if (n < 0) {
            perror("Error sending confirmation to client");
            return 1;
        }

        // Stop receiving if the matrix is complete
        if (row_count == MAX_ROWS) {
            printf("Matrix received completely. Stopping server...\n");
            break;
        }
    }

    // Step 7: Display the final matrix
    printf("Final Matrix:\n");
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    // Step 8: Close the server socket
    close(sockfd);
    return 0;
}
Client Code
#include<stdio.h>          // Standard input/output functions
#include<string.h>         // String handling functions
#include<stdlib.h>         // General utility functions like malloc
#include<sys/socket.h>     // Contains socket functions
#include<netinet/in.h>     // Contains constants and structures for internet domain addresses
#include<arpa/inet.h>      // For handling internet operations
#include<unistd.h>         // Contains POSIX API like close, sleep, etc.

#define PORTNO 10200       // The port number the server listens on
#define MAX_ROWS 10        // Maximum number of rows in the matrix
#define MAX_COLS 10        // Maximum number of columns in the matrix

int main() {
    int sockfd, n;
    struct sockaddr_in servaddr;             // Structure for server address
    socklen_t len;
    int matrix[MAX_ROWS][MAX_COLS];          // Matrix to send to the server
    char buffer[256];                        // Buffer for server confirmation

    // Step 1: Create the UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        return 1;
    }

    // Step 2: Initialize the server address structure
    servaddr.sin_family = AF_INET;           // Use IPv4 protocol
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost IP
    servaddr.sin_port = htons(PORTNO);       // Convert port number to network byte order
    len = sizeof(servaddr);

    // Step 3: Get matrix input from the user
    int row_count, col_count;
    printf("Enter number of rows and columns: ");
    scanf("%d %d", &row_count, &col_count);

    printf("Enter the matrix row by row:\n");
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            scanf("%d", &matrix[i][j]);     // Get matrix input
        }
    }

    // Step 4: Send each row to the server
    for (int i = 0; i < row_count; i++) {
        // Send one row to the server
        n = sendto(sockfd, matrix[i], col_count * sizeof(int), 0, (struct sockaddr *)&servaddr, len);
        if (n < 0) {
            perror("Error sending row to server");
            return 1;
        }

        // Step 5: Receive confirmation from the server
        n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&servaddr, &len);
        if (n < 0) {
            perror("Error receiving confirmation from server");
            return 1;
        }
        buffer[n] = '\0';  // Null-terminate the received string
        printf("%s\n", buffer);
    }

    // Step 6: Close the socket
    close(sockfd);
    return 0;
}
gcc -o udp_server udp_server.c
gcc -o udp_client udp_client.c
one terminal:
./udp_server
another terminal:
./udp_client
