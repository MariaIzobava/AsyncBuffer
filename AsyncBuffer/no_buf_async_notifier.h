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
	std::condition_variable pushed_;

	void execute() {
		while (true)
		{
			T local_value_;
			{
				std::unique_lock<std::mutex> lock(mx_);
				pushed_.wait(lock, [this]() {return base_notifier<T>::stopping || is_triggered_; });
				if (base_notifier<T>::stopping && !is_triggered_) break;
				std::swap(local_value_, value_);
				is_triggered_ = false;
			}


			base_notifier<T>::callback(local_value_);
		
			std::this_thread::sleep_for(std::chrono::seconds(1));

		}
	}
public:

	no_buf_async_notifier(void(*callback)(T)) : base_notifier<T>(callback) {
		th_ = std::thread(&no_buf_async_notifier<std::string>::execute, this);
	}

	void operator()(T val) {
		{
			std::lock_guard<std::mutex> lock(mx_);
			value_ = val;
			is_triggered_ = true;
		}
		pushed_.notify_one();
		
	}

	~no_buf_async_notifier() {
		base_notifier<T>::stop();
		th_.join();
	}
};

#endif