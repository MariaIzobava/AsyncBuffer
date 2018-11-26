#ifndef BUF_ASYNC_NOTIFIER_H
#define BUF_ASYNC_NOTIFIER_H
#include <mutex> 
#include <queue>
#include "base_notifier.h"

template <class T>
class buf_async_notifier : public base_notifier<T> {
private:

	std::queue<T> q_;

	std::thread th_;
	std::mutex mx_;
	std::mutex mx_for_locking_thread_;
	std::condition_variable pushed_;

	void execute() {
		while (true)
		{
			std::unique_lock<std::mutex> lock(mx_);
			pushed_.wait(lock, [&]() { return base_notifier<T>::stopping || !q_.empty(); });
			if (base_notifier<T>::stopping && q_.empty()) break;
			base_notifier<T>::callback(q_.front());
			q_.pop();

			std::this_thread::sleep_for(std::chrono::seconds(1));

		}
	}
public:

	buf_async_notifier(void(*callback)(T)) : base_notifier<T>(callback) {
		th_ = std::thread(&buf_async_notifier<int>::execute, this);
	}

	bool operator()(std::queue<T> &solver_queue) {
		if (q_.empty() && mx_.try_lock()) {
			std::swap(q_, solver_queue);
			mx_.unlock();
			pushed_.notify_one();
			return true;
		}
		return false;
	}

};

#endif