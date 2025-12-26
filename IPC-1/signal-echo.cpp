#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>

void handler(int, siginfo_t* info, void*) {
    passwd* pw = getpwuid(info->si_uid);
    std::cout << "Got SIGUSR1 from PID " << info->si_pid
              << ", UID " << info->si_uid
              << " (" << (pw ? pw->pw_name : "unknown") << ")\n";
}

int main() {
    std::cout << "PID = " << getpid() << "\n";

    struct sigaction sa{};
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigaction(SIGUSR1, &sa, nullptr);

    while (true) sleep(10);
}
