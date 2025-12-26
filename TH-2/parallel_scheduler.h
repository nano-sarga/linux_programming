#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class parallel_scheduler {
public:
    explicit parallel_scheduler(size_t thread_count);
    ~parallel_scheduler();

    void run(std::function<void()> task);

private:
    void worker();

    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

    std::mutex mutex;
    std::condition_variable cond;
    bool stop;
};

#endif

