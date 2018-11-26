#include <algorithm>
#include <string>
#include "buf_async_notifier.h"
3include "no_buf_async_notifier.h"

class Solver {
private:
	buf_async_notifier<int> _callback;
	no_buf_async_notifier<std::string> _logger;

public:
	Solver(void (*callback)(int), void(logger)(std::string))
	: _callback(callback), _logger(logger) {}

	void run() {
		for (int i = 0; i < 100; i++) {
			_callback(i);
			_logger(std::to_string(i));
		}
	}
};