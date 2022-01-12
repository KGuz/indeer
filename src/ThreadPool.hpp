#pragma once
#include <atomic>
#include <exception>
#include <functional>
#include <future>
#include <thread>
#include <type_traits>
#include <vector>
#include "ThreadSafeQueue.hpp"

class ThreadPool {
	std::atomic<bool> running;
	std::vector<std::thread> pool;
	ThreadSafeQueue <std::function<void()>> queue;

	void worker() {
		while (running) {
			std::function<void()> task;
			if (queue.try_pop(task)) {
				task();
			}
			else {
				std::this_thread::yield();
			}
		}
	};

public:
	ThreadPool(unsigned int pool_size = std::thread::hardware_concurrency()) : running(true) {
		pool.reserve(pool_size);
		for (auto i = 0; i < pool_size; ++i) {
			pool.push_back(std::thread(&ThreadPool::worker, this));
		};
	}

	~ThreadPool() {
		while (true) {
			if (queue.empty()) {
				break;
			}
			std::this_thread::yield();
		}

		running = false;
		for (auto& t : pool) {
			t.join();
		};
	}

	template <typename F, typename... A, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<A>...>>
	std::future<R> submit(const F& task, const A &...args) {
		std::shared_ptr<std::promise<R>> promise(new std::promise<R>);
		std::future<R> future = promise->get_future();

		queue.push([task, args..., promise]{
			try {
				promise->set_value(task(args...));
			}
			catch (...) {
				try {
					promise->set_exception(std::current_exception());
				}
				catch (...) {}
			}
			});
		return future;
	};
};