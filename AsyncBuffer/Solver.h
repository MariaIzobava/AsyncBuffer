#include <algorithm>
#include <string>
#include "buf_async_notifier.h"
#include "no_buf_async_notifier.h"
#include "queue_buffer.h"

class Solver {
private:
	buf_async_notifier<int> _callback;
	no_buf_async_notifier<std::string> _logger;
	queue_buffer<int> _new_callback;

public:
	Solver(void (*callback)(int), void(logger)(std::string), void(*new_callback)(int))
	: _callback(callback), _logger(logger), _new_callback(new_callback){}

	void run() {
		for (int i = 0; i < 5; i++) {
			//_callback(i);
			//_logger(std::to_string(i));
			_new_callback(i);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		std::cout << "Stop generating data\n";
	}
};