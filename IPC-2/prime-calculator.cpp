#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cstring>

int nth_prime(int m) {
    if (m < 1) 
	    return -1;
    int count = 0;
    int num = 2;
    while (true) {
        bool isPrime = true;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) { 
		    isPrime = false; 
		    break;
	    }
        }
        if (isPrime) 
		count++;
        if (count == m) 
		return num;
        num++;
    }
}

int main() {
    int parent_to_child[2];
    int child_to_parent[2];

    if (pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        while (true) {
            int m;
            ssize_t r = read(parent_to_child[0], &m, sizeof(int));
            if (r <= 0) break;

            if (m == -1) break;

            std::cout << "[Child] Calculating " << m << "-th prime number...\n";
            int result = nth_prime(m);

            std::cout << "[Child] Sending calculation result of prime(" 
                      << m << ")...\n";

            ssize_t w = write(child_to_parent[1], &result, sizeof(int));
            if (w <= 0) break;
        }

        close(parent_to_child[0]);
        close(child_to_parent[1]);
        return 0;
    } 
    else {
        close(parent_to_child[0]);
        close(child_to_parent[1]);

        while (true) {
            std::string input;
            std::cout << "[Parent] Please enter the number: ";
            std::cin >> input;

            if (input == "exit") {
                int cmd = -1;
                ssize_t w = write(parent_to_child[1], &cmd, sizeof(int));
                (void)w;
                break;
            }

            int m = std::stoi(input);

            std::cout << "[Parent] Sending " << m << " to the child process...\n";
            ssize_t w1 = write(parent_to_child[1], &m, sizeof(int));
            (void)w1;

            std::cout << "[Parent] Waiting for the response from the child process...\n";
            int result;
            ssize_t r1 = read(child_to_parent[0], &result, sizeof(int));
            (void)r1;

            std::cout << "[Parent] Received calculation result of prime(" 
                      << m << ") = " << result << "...\n";
        }

        close(parent_to_child[1]);
        close(child_to_parent[0]);

        return 0;
    }
}
