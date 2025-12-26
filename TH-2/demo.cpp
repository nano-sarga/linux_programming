#include "parallel_scheduler.h"
#include <iostream>
#include <unistd.h>

int main() {
    parallel_scheduler pool(3);

    for (int i = 0; i < 10; ++i) {
        pool.run([i]() {
            std::cout << "Task " << i
                      << " running in thread "
                      << std::this_thread::get_id()
                      << std::endl;
            sleep(1);
        });
    }

    sleep(5);
    return 0;
}

