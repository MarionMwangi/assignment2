#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/example_socket"

int main() {
    int client_fd;
    struct sockaddr_un addr;
    char message[] = "Hello Server";
    char buffer[128];

    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    connect(client_fd, (struct sockaddr*)&addr, sizeof(addr));
    send(client_fd, message, strlen(message) + 1, 0);
    recv(client_fd, buffer, sizeof(buffer), 0);

    printf("Client received: %s\n", buffer);

    close(client_fd);
    return 0;
}
