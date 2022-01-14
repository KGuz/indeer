#include "ThreadPool.h"

// Initialize worker threads
ThreadPool::ThreadPool(unsigned int pool_size) : running(true) {
    pool_size = std::clamp(pool_size, 1u, std::thread::hardware_concurrency());
    pool.reserve(pool_size);
    for (unsigned int i = 0; i < pool_size; ++i) {
        pool.push_back(std::thread(&ThreadPool::worker, this));
    };
}

// Waits for threads to finish tasks and terminates them
ThreadPool::~ThreadPool() {
    while (true) {
        if (queue.empty()) {
            break;
        }
        std::this_thread::yield();
    }

    running = false;
    for (auto &t : pool) {
        t.join();
    };
}

// Infinite loop function for threads to retrieve tasks from the queue
void ThreadPool::worker() {
    while (running) {
        std::function<void()> task;
        if (queue.try_pop(task)) {
            task();
        } else {
            std::this_thread::yield();
        }
    }
};