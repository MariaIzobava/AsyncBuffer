#ifndef BUF_ASYNC_NOTIFIER_H
#define BUF_ASYNC_NOTIFIER_H
#include <mutex> 
#include <queue>
#include "base_notifier.h"

template <class T>
class buf_async_notifier : public base_notifier<T> {
private:

	std::queue<T> thread_q_;

	std::thread th_;
	std::mutex mx_;
	std::condition_variable pushed_;

	void execute() {
		while (true)
		{
			std::queue<T> local_q_;
			{
				std::unique_lock<std::mutex> lock(mx_);
				pushed_.wait(lock, [this]() {return base_notifier<T>::stopping || !thread_q_.empty(); });
				if (base_notifier<T>::stopping && thread_q_.empty()) break;
				std::swap(local_q_, thread_q_);
			}

			while (!local_q_.empty()) {
				base_notifier<T>::callback(local_q_.front());
				local_q_.pop();
				std::this_thread::sleep_for(std::chrono::seconds(2));
			}

			
		}
	}
public:

	buf_async_notifier(void(*callback)(T)) : base_notifier<T>(callback) {
		th_ = std::thread(&buf_async_notifier<int>::execute, this);
	}

	void operator()(T val) {
		{
			std::unique_lock<std::mutex> lock(mx_);
			thread_q_.push(val);
		}
		pushed_.notify_one();
	}

	~buf_async_notifier() {
		base_notifier<T>::stop();
		th_.join();
	}

};

#endif