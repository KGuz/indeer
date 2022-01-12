#pragma once
#include <mutex>
#include <queue>

template <typename T>
class ThreadSafeQueue {
	std::queue<T> q;
	std::mutex m;

public:
	void push(const T& value) {
		std::unique_lock<std::mutex> lock(m);
		q.push(value);
	}

	bool try_pop(T& value) {
		std::unique_lock<std::mutex> lock(m);
		if (q.empty()) {
			return false;
		}
		value = q.front();
		q.pop();
		return true;
	}

	bool empty() {
		std::unique_lock<std::mutex> lock(m);
		return q.empty();
	}
};