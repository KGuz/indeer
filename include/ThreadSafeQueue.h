#pragma once
#include <mutex>
#include <queue>

/**
  Simple implementation of a thread safe queue.
  Contains mathod for pushing and poping task from queue without dataraces 
  @tparam T the type of data stored in the queue
*/
template <typename T>
class ThreadSafeQueue {
    std::queue<T> q;
    std::mutex m;

  public:
    /**
	  Inserts data into the queue
	  @param item Element to be queued 
	*/
    void push(const T &item) {
        std::scoped_lock<std::mutex> lock(m);
        q.push(item);
    }

    /**
	  Tries to retrieve data from the queue
	  @param item Retrived element
	  @return True if the operation was successful, otherwise false 
	*/
    bool try_pop(T &item) {
        std::scoped_lock<std::mutex> lock(m);
        if (q.empty()) {
            return false;
        }
        item = q.front();
        q.pop();
        return true;
    }

    /**
	  Checks whether queue is empty or not
	  @return True if the queue is empty 
	*/
    bool empty() {
        std::scoped_lock<std::mutex> lock(m);
        return q.empty();
    }
};