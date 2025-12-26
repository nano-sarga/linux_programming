#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#define PORT 5555
#define BUF_SIZE 1024

int client_fd;

void* receive_thread(void*) {
    char buffer[BUF_SIZE];
    while (true) {
        int bytes_received = recv(client_fd, buffer, BUF_SIZE - 1, 0);
        if (bytes_received <= 0) break;
        buffer[bytes_received] = '\0';
        std::cout << buffer;
    }
    exit(0);
}

int main() {
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(client_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed\n";
        return 1;
    }

    std::string username;
    std::cout << "Enter your name: ";
    std::getline(std::cin, username);
    username += "\n";
    send(client_fd, username.c_str(), username.size(), 0);

    pthread_t tid;
    pthread_create(&tid, nullptr, receive_thread, nullptr);

    std::string message;
    while (true) {
        std::getline(std::cin, message);
        message += "\n";
        send(client_fd, message.c_str(), message.size(), 0);
        if (message == "/exit\n") break;
    }

    close(client_fd);
}

