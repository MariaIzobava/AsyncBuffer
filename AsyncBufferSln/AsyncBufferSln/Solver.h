#include <algorithm>
#include <string>
#include "queue_buffer.h"
#include "optional_buffer.h"

class Solver {
private:
	queue_buffer<int> buff_callback_;
	optional_buffer<std::string> optional_callback_;

public:
	Solver(void(*new_callback)(int), void(*callback)(std::string))
	: buff_callback_(new_callback), optional_callback_(callback){}

	void run() {
		for (int i = 0; i < 5; i++) {
			//_callback(i);
			//_logger(std::to_string(i));
			optional_callback_(std::to_string(i));
			buff_callback_(i);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		std::cout << "Stop generating data\n";
	}
};