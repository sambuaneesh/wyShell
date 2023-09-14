#include "headers.h"

// Function to send an HTTP GET request and retrieve the response
char *httpGet(const char *url) {
    char *response = NULL;
    int response_size = 1; // Start with space for null-terminator

    // Parse the URL to extract host and path
    char host[256];
    char path[256];
    if (sscanf(url, "http://%255[^/]/%255[^\n]", host, path) != 2) {
        printError("Invalid URL format\n");
        return NULL;
    }

    // Resolve host to IP address
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, "http", &hints, &res) != 0) {
        printError("Failed to resolve host\n");
        return NULL;
    }

    // Create a socket
    int sockfd = socket(res->ai_family, res->ai_socktype, 0);
    if (sockfd == -1) {
        printError("socket");
        freeaddrinfo(res);
        return NULL;
    }

    // Connect to the server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        printError("connect");
        close(sockfd);
        freeaddrinfo(res);
        return NULL;
    }

    // Send HTTP GET request
    char request[512];
    sprintf(request, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", path, host);
    if (send(sockfd, request, strlen(request), 0) == -1) {
        printError("send");
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
            printError("realloc");
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


int noCommandFound(const char *text) {
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

void iMan(Command *cmd) {
    if (cmd->argc != 2) {
        printError("ERROR\n\tUsage: iMan <command>\n");
        return;
    }
    char *command = cmd->argv[1];
    char url[512];
    sprintf(url, "http://man.he.net/?topic=%s&section=all", command);

    char *manPageHTML = httpGet(url);

    if (manPageHTML) {
        // Locate "NAME" section
        char *nameStart = strstr(manPageHTML, "NAME\n");

        if (nameStart) {
            // Print from "NAME" to "AUTHOR" if "AUTHOR" is found
            char *authorStart = strstr(manPageHTML, "AUTHOR");
            if (authorStart && authorStart > nameStart) {
                size_t section_length = authorStart - nameStart;
                printf("%.*s\n", (int) section_length, nameStart);
            }
                // Print from "NAME" to "SEE ALSO" if "AUTHOR" is not found
            else {
                char *seealsoStart = strstr(manPageHTML, "SEE ALSO");
                if (seealsoStart && seealsoStart > nameStart) {
                    size_t section_length = seealsoStart - nameStart;
                    printf("%.*s\n", (int) section_length, nameStart);
                } else {
                    // "SEE ALSO" not found, print the whole "NAME" section
                    printf("%s\n", nameStart);
                }
            }
        } else {
            printError("ERROR\n\tSection not found\n");
        }

        free(manPageHTML);
    } else {
        printError("Failed to retrieve the man page HTML\n");
    }
}
