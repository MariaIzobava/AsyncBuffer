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

	void execute() {
		while (!base_notifier<T>::stopping)
		{

			while (!is_triggered_) {}

			base_notifier<T>::callback(value_);
			is_triggered_ = false;

			std::this_thread::sleep_for(std::chrono::seconds(1));

		}
	}
public:

	no_buf_async_notifier(void(*callback)(T)) : base_notifier<T>(callback) {
		th_ = std::thread(&no_buf_async_notifier<std::string>::execute, this);
	}

	void operator()(T val) {
		std::lock_guard<std::mutex> lock(mx_);
		value_ = val;
		is_triggered_ = true;
	}

};

#endif