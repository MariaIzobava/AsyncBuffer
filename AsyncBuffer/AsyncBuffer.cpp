#include "pch.h"
#include <iostream>
#include <thread>
#include "QueueBuffer.h"
#include "CountBuffer.h"
#include "LogBuffer.h"
#include <vector>
#include <mutex>
#include "BufferNotifier.h"

void f1(int t) {
	std::cout << "Generated element" << ": " << t << std::endl;
}

void f2(int const & k) {
	std::cout << "Number of generated elements" << ": " << k << std::endl;
}

void f3() {
	std::cout << "Some LOG sentence" << std::endl;

}

int main()
{
	BufferNotifier bf;
	bf.add_queue_buffer<int, std::function<void(int)>>(f1);
	bf.add_count_buffer<std::function<void(int const &)>>(f2);
	bf.add_log_buffer<std::function<void()>>(f3);

	for (int i = 1; i <= 5; i++) {
		bf.add_value<int>(i);
		if (i % 2)
			bf.add_log_message();
	}
	getchar();
}
