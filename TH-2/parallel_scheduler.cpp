#include "parallel_scheduler.h"

parallel_scheduler::parallel_scheduler(size_t thread_count)
    : stop(false)
{
    for (size_t i = 0; i < thread_count; ++i) {
        threads.emplace_back(&parallel_scheduler::worker, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    {
        std::lock_guard<std::mutex> lock(mutex);
        stop = true;
    }

    cond.notify_all();

    for (auto& t : threads) {
        if (t.joinable())
            t.join();
    }
}

void parallel_scheduler::run(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        tasks.push(std::move(task));
    }
    cond.notify_one();
}

void parallel_scheduler::worker() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mutex);
            cond.wait(lock, [&] {
                return stop || !tasks.empty();
            });

            if (stop && tasks.empty())
                return;

            task = std::move(tasks.front());
            tasks.pop();
        }

        task();
    }
}

