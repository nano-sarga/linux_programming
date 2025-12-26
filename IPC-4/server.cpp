#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

#define PORT 5555
#define BUF_SIZE 1024

struct Client {
    int fd;
    std::string name;
};

std::vector<Client> clients;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast(const std::string& msg, int sender_fd) {
    pthread_mutex_lock(&clients_mutex);
    for (auto& c : clients)
        if (c.fd != sender_fd)
            send(c.fd, msg.c_str(), msg.size(), 0);
    pthread_mutex_unlock(&clients_mutex);
}

void send_user_list(int client_fd) {
    std::string list = "Users:\n";
    pthread_mutex_lock(&clients_mutex);
    for (auto& c : clients)
        list += " - " + c.name + "\n";
    pthread_mutex_unlock(&clients_mutex);
    send(client_fd, list.c_str(), list.size(), 0);
}

void remove_client(int client_fd) {
    pthread_mutex_lock(&clients_mutex);
    clients.erase(std::remove_if(clients.begin(), clients.end(),
                 [&](const Client& c){ return c.fd == client_fd; }), clients.end());
    pthread_mutex_unlock(&clients_mutex);
}

void* client_thread(void* arg) {
    int client_fd = *(int*)arg;
    delete (int*)arg;

    char buffer[BUF_SIZE];
    int bytes_received = recv(client_fd, buffer, BUF_SIZE - 1, 0);
    if (bytes_received <= 0) { close(client_fd); return nullptr; }

    buffer[bytes_received] = '\0';
    std::string name(buffer);
    name.erase(std::remove(name.begin(), name.end(), '\n'), name.end());
    name.erase(std::remove(name.begin(), name.end(), '\r'), name.end());

    pthread_mutex_lock(&clients_mutex);
    clients.push_back({client_fd, name});
    pthread_mutex_unlock(&clients_mutex);

    broadcast(name + " joined the chat\n", client_fd);

    while (true) {
        bytes_received = recv(client_fd, buffer, BUF_SIZE - 1, 0);
        if (bytes_received <= 0) break;

        buffer[bytes_received] = '\0';
        std::string message(buffer);

        if (message == "/exit\n") break;
        if (message == "/list\n") { send_user_list(client_fd); continue; }

        broadcast(name + ": " + message, client_fd);
    }

    broadcast(name + " left the chat\n", client_fd);
    close(client_fd);
    remove_client(client_fd);
    return nullptr;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 10);

    std::cout << "Server started on port " << PORT << std::endl;

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        int* fd_ptr = new int(client_fd);
        pthread_t tid;
        pthread_create(&tid, nullptr, client_thread, fd_ptr);
        pthread_detach(tid);
    }
}

