#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    while (1) {
        bzero(buffer, BUFFER_SIZE);
        read(client_socket, buffer, BUFFER_SIZE);
        sleep(3);

        printf("Request received: %s\n", buffer);

        if (strcmp(buffer, "get") == 0) {
            char *file_content = "This is the content of the file from Production Server.\n";
            write(client_socket, file_content, strlen(file_content));

        } else if (strcmp(buffer, "list") == 0) {
            DIR *d;
            struct dirent *dir;
            d = opendir("ProductionData");
            if (d) {
                while ((dir = readdir(d)) != NULL) {
                    if (strstr(dir->d_name, ".txt") != NULL) {
                        write(client_socket, dir->d_name, strlen(dir->d_name));
                        write(client_socket, "\n", 1);
                    }
                }
                closedir(d);
            }
        } else if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }
    close(client_socket);
}

void statistiques_errorReport(char *erreur) {
    time_t raw_time;
    time(&raw_time);

    struct tm *time_info;
    time_info = localtime(&raw_time);

    char time_str[20];
    strftime(time_str, sizeof(time_str), "%H:%M %d-%m-%Y", time_info);

    FILE *file = fopen("ProductionStat.txt", "a");
    fprintf(file, "[%s] ERROR: %s\n", time_str, erreur);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        statistiques_errorReport("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        statistiques_errorReport("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        statistiques_errorReport("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        statistiques_errorReport("listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Production Server is running...\n");

    while ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) >= 0) {
        handle_client(client_socket);
    }

    return 0;
}
