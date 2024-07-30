#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>

#define PROD_PORT 8080
#define BACKUP_PORT 8081
#define BUFFER_SIZE 1024

int test_server(const char *ip, int port) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *test_message = "exit";
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return 0;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        return 0;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        return 0;
    }

    send(sock, test_message, strlen(test_message), 0);
    close(sock);
    return 1;
}

void copy_file(const char *filename) {
    int prod_sock, backup_sock;
    struct sockaddr_in prod_addr, backup_addr;
    char buffer[BUFFER_SIZE] = {0};
    char file_request[BUFFER_SIZE];

    sprintf(file_request, "get %s", filename);

    if ((prod_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Production socket creation error");
        return;
    }

    prod_addr.sin_family = AF_INET;
    prod_addr.sin_port = htons(PROD_PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &prod_addr.sin_addr) <= 0) {
        perror("Invalid address for Production server");
        return;
    }
    if (connect(prod_sock, (struct sockaddr *)&prod_addr, sizeof(prod_addr)) < 0) {
        perror("Connection to Production server failed");
        return;
    }

    // Request file from Production Server
    send(prod_sock, file_request, strlen(file_request), 0);
    read(prod_sock, buffer, BUFFER_SIZE);
    close(prod_sock);

    // Connect to Backup Server
    if ((backup_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Backup socket creation error");
        return;
    }
    backup_addr.sin_family = AF_INET;
    backup_addr.sin_port = htons(BACKUP_PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &backup_addr.sin_addr) <= 0) {
        perror("Invalid address for Backup server");
        return;
    }
    if (connect(backup_sock, (struct sockaddr *)&backup_addr, sizeof(backup_addr)) < 0) {
        perror("Connection to Backup server failed");
        return;
    }

    // Send file to Backup Server
    send(backup_sock, "send_file", strlen("send_file"), 0);
    send(backup_sock, buffer, strlen(buffer), 0);
    close(backup_sock);

    printf("File %s sent to Backup Server and deleted from Integration Server.\n", filename);
}

void synchronize_files() {
    int prod_sock, backup_sock;
    struct sockaddr_in prod_addr, backup_addr;
    char buffer[BUFFER_SIZE] = {0};
    char prod_files[BUFFER_SIZE] = {0};
    char backup_files[BUFFER_SIZE] = {0};

    // Connect to Production Server to list files
    if ((prod_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Production socket creation error");
        return;
    }
    prod_addr.sin_family = AF_INET;
    prod_addr.sin_port = htons(PROD_PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &prod_addr.sin_addr) <= 0) {
        printf("Invalid address for Production server");
        return;
    }
    if (connect(prod_sock, (struct sockaddr *)&prod_addr, sizeof(prod_addr)) < 0) {
        printf("Connection to Production server failed");
        return;
    }
    send(prod_sock, "list", strlen("list"), 0);
    read(prod_sock, prod_files, BUFFER_SIZE);
    close(prod_sock);

    // Connect to Backup Server to list files
    if ((backup_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Backup socket creation error");
        return;
    }
    backup_addr.sin_family = AF_INET;
    backup_addr.sin_port = htons(BACKUP_PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &backup_addr.sin_addr) <= 0) {
        perror("Invalid address for Backup server");
        return;
    }
    if (connect(backup_sock, (struct sockaddr *)&backup_addr, sizeof(backup_addr)) < 0) {
        perror("Connection to Backup server failed");
        return;
    }
    send(backup_sock, "list", strlen("list"), 0);
    read(backup_sock, backup_files, BUFFER_SIZE);
    close(backup_sock);

    // Compare file lists and identify missing files
    char *prod_file = strtok(prod_files, "\n");
    while (prod_file != NULL) {
        if (!strstr(backup_files, prod_file)) {
            copy_file(prod_file);
        }
        prod_file = strtok(NULL, "\n");
    }
}

int main() {
    /*
    if (test_server("127.0.0.1", PROD_PORT)) {
        printf("Production Server is available.\n");
    } else {
        printf("Production Server is not available.\n");
    }

    if (test_server("127.0.0.1", BACKUP_PORT)) {
        printf("Backup Server is available.\n");
    } else {
        printf("Backup Server is not available.\n");
    }
    */

    //synchronize_files();

    copy_file("example.txt");

    return 0;
}
