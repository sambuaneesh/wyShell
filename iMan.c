#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void fetchManPage(const char *command_name) {
    struct hostent *server;
    struct sockaddr_in server_addr;
    int sockfd, portno;
    char buffer[BUFFER_SIZE];

    // DNS resolution for man.he.net
    server = gethostbyname("man.he.net");
    if (server == NULL) {
        fprintf(stderr, "ERROR: Could not resolve host\n");
        exit(1);
    }

    // Open a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    portno = 80;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portno);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        error("ERROR connecting");
    }

    // Send a GET request to the website's server
    sprintf(buffer, "GET /%s HTTP/1.1\r\nHost: man.he.net\r\n\r\n", command_name);
    if (write(sockfd, buffer, strlen(buffer)) < 0) {
        error("ERROR writing to socket");
    }

    // Read and print the body of the website
    memset(buffer, 0, sizeof(buffer));
    int n;
    while ((n = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) {
        printf("%s", buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    // Close the socket
    close(sockfd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <command_name>\n", argv[0]);
        return 1;
    }

    const char *command_name = argv[1];
    fetchManPage(command_name);

    return 0;
}
