#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "ctype.h"

// Function to send an HTTP GET request and retrieve the response
char *httpGet(const char *url) {
    char *response = NULL;
    int response_size = 1; // Start with space for null-terminator

    // Parse the URL to extract host and path
    char host[256];
    char path[256];
    if (sscanf(url, "http://%255[^/]/%255[^\n]", host, path) != 2) {
        fprintf(stderr, "Invalid URL format\n");
        return NULL;
    }

    // Resolve host to IP address
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, "http", &hints, &res) != 0) {
        fprintf(stderr, "Failed to resolve host\n");
        return NULL;
    }

    // Create a socket
    int sockfd = socket(res->ai_family, res->ai_socktype, 0);
    if (sockfd == -1) {
        perror("socket");
        freeaddrinfo(res);
        return NULL;
    }

    // Connect to the server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        close(sockfd);
        freeaddrinfo(res);
        return NULL;
    }

    // Send HTTP GET request
    char request[512];
    sprintf(request, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", path, host);
    if (send(sockfd, request, strlen(request), 0) == -1) {
        perror("send");
        close(sockfd);
        freeaddrinfo(res);
        return NULL;
    }

    // Receive and store the response
    char buffer[1024];
    response = malloc(response_size); // Allocate space for null-terminator
    response[0] = '\0'; // Initialize the response string

    while (1) {
        int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) break;
        buffer[bytes_received] = '\0';

        // Calculate the new size
        int new_size = response_size + bytes_received;

        // Reallocate memory
        response = realloc(response, new_size);

        if (response == NULL) {
            perror("realloc");
            free(response);
            close(sockfd);
            freeaddrinfo(res);
            return NULL;
        }

        strcat(response, buffer);
        response_size = new_size;
    }

    // Clean up and close the socket
    close(sockfd);
    freeaddrinfo(res);

    return response;
}

// Function to remove HTML tags from a string
void removeHtmlTags(char *html) {
    int i = 0;
    int j = 0;
    int insideTag = 0;

    while (html[i]) {
        if (html[i] == '<') {
            insideTag = 1;
        } else if (html[i] == '>') {
            insideTag = 0;
        } else if (!insideTag) {
            html[j++] = html[i];
        }
        i++;
    }

    html[j] = '\0';
}

// Function to skip the first n lines from a string
void skipLines(char *text, int n) {
    int i = 0;
    while (n > 0 && text[i]) {
        if (text[i] == '\n') {
            n--;
        }
        i++;
    }

    // Copy the rest of the string
    int j = 0;
    while (text[i]) {
        text[j++] = text[i++];
    }
    text[j] = '\0';
}

void removeLastNLines(char *text, int n) {
    if (n <= 0) {
        return;  // Nothing to remove
    }

    int len = strlen(text);
    int linesRemoved = 0;

    for (int i = len - 1; i >= 0; i--) {
        if (text[i] == '\n') {
            linesRemoved++;
            if (linesRemoved == n) {
                text[i] = '\0';  // Null-terminate the string at this point
                break;
            }
        }
    }
}

int checkManPageText(const char *text) {
    const char *pattern = "Man Pages Copyright Respective Owners.  Site Copyright (C) 1994 - 2023\nHurricane Electric.\nAll Rights Reserved.";

    // Remove trailing spaces, tabs, and newlines before checking
    int len = strlen(text);
    while (len > 0 && (isspace(text[len - 1]) || text[len - 1] == '\n')) {
        len--;
    }

    // Compare the cleaned text with the pattern
    if (strncmp(text, pattern, len) == 0 && pattern[len] == '\0') {
        return 1; // Match found
    } else {
        return 0; // No match
    }
}

int main() {
    const char *command = "strstr";  // Replace with the user's input
    char url[512];
    sprintf(url, "http://man.he.net/?topic=%s&section=all", command);

    char *manPageHTML = httpGet(url);

    if (manPageHTML) {
        // Skip the first 19 lines
        skipLines(manPageHTML, 18);

        // Remove HTML tags from manPageHTML
        removeHtmlTags(manPageHTML);

        // Remove the last 5 lines
        removeLastNLines(manPageHTML, 20);

        if (checkManPageText(manPageHTML)) {
            fprintf(stderr, "ERROR\n\tNo such command");
        } else {
            printf("%s\n", manPageHTML);
            free(manPageHTML);
        }
    } else {
        printf("Failed to retrieve the man page HTML\n");
    }

    return 0;
}
