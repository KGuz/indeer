#pragma once
#include "ThreadSafeQueue.h"
#include <algorithm>
#include <atomic>
#include <exception>
#include <functional>
#include <future>
#include <thread>
#include <type_traits>
#include <vector>

/**
  Simple thread pool implementation
  Contains methods for creating threads and submitting tasks to a job queue
*/
class ThreadPool {
    std::atomic<bool> running;
    std::vector<std::thread> pool;
    ThreadSafeQueue<std::function<void()>> queue;

    /// Infinite loop function for threads to retrieve tasks from the queue
    void worker();

  public:
    /**
	  Initialize worker threads
	  @param pool_size Number of spawned threads
	*/
    ThreadPool(unsigned int pool_size);

    /// Waits for threads to finish tasks and terminates them
    ~ThreadPool();

    /** 
	  Passes given task to the job queue
	  @tparam F The type of function 'task'
	  @tparam A The type of arguments to the "task" function 
	  @tparam R The type of the return value of the 'task' function called with arguments 'args'
	  @param task Function to be executed by one of the threads
	  @param args Arguments with which the 'task' function will be called
	  @return Future object containing result of the'task' function called with arguments 'args'
	*/
    template <typename F, typename... A, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<A>...>>
    std::future<R> submit(const F &task, const A &... args) {
        std::shared_ptr<std::promise<R>> promise(new std::promise<R>);
        std::future<R> future = promise->get_future();

        queue.push([task, args..., promise] {
            try {
                promise->set_value(task(args...));
            } catch (...) {
                try {
                    promise->set_exception(std::current_exception());
                } catch (...) {
                }
            }
        });
        return future;
    };
};