#ifndef BUF_ASYNC_NOTIFIER_H
#define BUF_ASYNC_NOTIFIER_H
#include <mutex> 
#include <queue>
#include "base_notifier.h"

template <class T>
class buf_async_notifier : base_notifier<T> {
private:

	std::queue<T> q_;

	std::thread th_;
	std::mutex mx_;

	void execute() {
		while (!g_stopping)
		{
			
			while (q_.empty()) {}

			_callback(q_.front());
			q_.pop();

			//std::this_thread::sleep_for(std::chrono::seconds(1));

		}
	}
public:

	buf_async_notifier(void(*callback)(T)) : base_buffer(callback) {
		th_ = std::thread(execute);
	}

	void operator()(T val) {
		std::lock_guard<std::mutex> lock(mx_);
		q_.push(val);
	}

	
};

#endif