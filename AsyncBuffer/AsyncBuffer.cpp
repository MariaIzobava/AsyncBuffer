#include "pch.h"
#include <iostream>
#include <thread>
#include "QueueBuffer.h"
#include "CountBuffer.h"
#include "LogBuffer.h"
#include <vector>
#include <mutex>

void f1(int t) {
	std::cout << __FUNCSIG__ << ": " << t << std::endl;
}

void f2(int const & k) {
	std::cout << __FUNCSIG__ << ": " << k << std::endl;
}

void f3() {
	std::cout << __FUNCSIG__ << std::endl;

}

const int n_threads = 3;

std::queue<int> g_values;
bool g_stopping = false;
std::mutex g_value_mx;
std::condition_variable g_cv_produced;
std::condition_variable g_cv_consumed;

template <class TFunc>
void receiver(BaseBuffer<TFunc>* buf)
{
	while (!g_stopping)
	{
		{
			std::unique_lock<std::mutex> lock(g_value_mx);
			g_cv_produced.wait(
				lock,
				[&]() { return g_stopping || buf->is_triggered(); });

			if (g_stopping) return;

			buf->callback();
		}

		g_cv_consumed.notify_all();

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void stop()
{
	{
		std::lock_guard<std::mutex> lock(g_value_mx);
		g_stopping = true;
	}

	g_cv_produced.notify_all();
}



int main()
{

	QueueBuffer<int, std::function<void(int)>> buf1(f1);
	CountBuffer<std::function<void(int const &)>> buf2(f2);
	LogBuffer<std::function<void()>> buf3(f3);
	/*buf1.add(45);
	buf2.add();
	buf1.callback();
	buf2.callback();
	buf3.callback();*/

	std::vector<std::thread> receivers;
	receivers.reserve(n_threads);
	receivers.emplace_back(receiver<std::function<void(int)>>, &buf1);
	receivers.emplace_back(receiver<std::function<void(int const &)>>, &buf2);
	receivers.emplace_back(receiver<std::function<void()>>, &buf3);

	for (int i = 1; i <= 10; i++) {
		{
			std::lock_guard<std::mutex> lock(g_value_mx);
			//buf1.add(i);
			buf2.add();
		}
		g_cv_produced.notify_all();

	}

	{
		std::lock_guard<std::mutex> lock(g_value_mx);
		std::cout << "The generator has finished to produce data\n";
	}

	{
		std::unique_lock<std::mutex> lock(g_value_mx);
		g_cv_consumed.wait(lock, [&]() { return buf1.is_empty() && !buf1.is_triggered(); });
	}

	stop();

	receivers[0].join();
	receivers[1].join();
	receivers[2].join();
	getchar();
}
