// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    
    double alice_balance = 1000.0;
    double bob_balance = 500.0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("Socket failed"); return 1; }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    listen(server_fd, 1);
    printf("Server listening on port %d...\n", port);

    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
    printf("Client connected!\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = recv(client_fd, buffer, sizeof(buffer), 0);
        if (n <= 0) break;

        buffer[strcspn(buffer, "\n")] = 0; 

        if (strncmp(buffer, "BALANCE", 7) == 0) {
            sprintf(buffer, "Alice: %.2f, Bob: %.2f\n", alice_balance, bob_balance);
            send(client_fd, buffer, strlen(buffer), 0);
        } 
        else if (strncmp(buffer, "SEND", 4) == 0) {
            char from[10], to[10];
            double amount;
            sscanf(buffer, "SEND %s %s %lf", from, to, &amount);

            if (strcmp(from, "alice") == 0 && alice_balance >= amount) {
                alice_balance -= amount;
                bob_balance += amount;
                sprintf(buffer, "Alice sent %.2f to Bob. New Balances -> Alice: %.2f, Bob: %.2f\n",
                        amount, alice_balance, bob_balance);
            } 
            else {
                sprintf(buffer, "Transaction failed. Check balance or account name.\n");
            }
            send(client_fd, buffer, strlen(buffer), 0);
        }
        else if (strcmp(buffer, "QUIT") == 0) {
            send(client_fd, "Goodbye!\n", 9, 0);
            break;
        }
        else {
            send(client_fd, "Unknown command.\n", 17, 0);
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

