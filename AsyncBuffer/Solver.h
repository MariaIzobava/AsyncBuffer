#include <algorithm>
#include <string>
#include "buf_async_notifier.h"
#include "no_buf_async_notifier.h"
#include <queue>

class Solver {
private:
	buf_async_notifier<int> _callback;
	no_buf_async_notifier<std::string> _logger;

	std::queue<int> local_queue_;
	std::string local_logger_message_;

public:
	Solver(void (*callback)(int), void(logger)(std::string))
	: _callback(callback), _logger(logger) {}

	void run() {
		for (int i = 0; i < 5; i++) {
			local_queue_.push(i);
			_callback(local_queue_);

			local_logger_message_ = std::to_string(i+1);
			if (_logger(local_logger_message_))
				local_logger_message_.clear();

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		if (!local_queue_.empty())
			while (!_callback(local_queue_)) {}
		if  (!local_logger_message_.empty())
			while (!_logger(local_logger_message_)) {}
		_callback.stop();
		_logger.stop();
	}
};