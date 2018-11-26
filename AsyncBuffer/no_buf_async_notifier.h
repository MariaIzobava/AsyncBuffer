#ifndef NO_BUF_ASYNC_NOTIFIER_H
#define NO_BUF_ASYNC_NOTIFIER_H

#include "base_notifier.h"
#include <thread>
#include <mutex>

template <class T>
class no_buf_async_notifier: public base_notifier<T> {

private:
	void(*_callback)(T);
	T value_;
	bool is_triggered_ = false;

	std::thread th_;
	std::mutex mx_;
	std::mutex mx_for_locking_thread_;
	std::condition_variable pushed_;

	void execute() {
		while (true)
		{
			std::unique_lock<std::mutex> lock(mx_);
			pushed_.wait(lock, [&]() { return base_notifier<T>::stopping || is_triggered_; });
			if (base_notifier<T>::stopping && !is_triggered_) break;
			base_notifier<T>::callback(value_);
			is_triggered_ = false;

			std::this_thread::sleep_for(std::chrono::seconds(1));

		}
	}
public:

	no_buf_async_notifier(void(*callback)(T)) : base_notifier<T>(callback) {
		th_ = std::thread(&no_buf_async_notifier<std::string>::execute, this);
	}

	bool operator()(T val) {
		//std::lock_guard<std::mutex> lock(mx_);
		if (!is_triggered_ && !val.empty() && mx_.try_lock()) {
			value_ = val;
			is_triggered_ = true;
			mx_.unlock();
			pushed_.notify_one();
			return true;
		}
		return false;

	}


};

#endif