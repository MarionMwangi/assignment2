#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/example_socket"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[256];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    unlink(SOCKET_PATH);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    recv(client_fd, buffer, sizeof(buffer), 0);
    printf("Server received: %s\n", buffer);

    const char reply[] = "Hello Client";
    send(client_fd, reply, sizeof(reply), 0);

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
