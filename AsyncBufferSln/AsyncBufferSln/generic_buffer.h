#ifndef GENERIC_BUFFER_H
#define GENERIC_BUFFER_H

#include <mutex>
#include <queue>

struct one_shot {

private:
	std::condition_variable cv_;
	std::mutex mx_;
	bool f = false;

public:
	void wait() {
		std::unique_lock<std::mutex> lock(mx_);
		cv_.wait(lock, [&]() {return f; });
	}

	void signal() {
		{
			std::lock_guard<std::mutex> lock(mx_);
			f = true;
		}

		cv_.notify_all();
	}
};

template <class T, class S, class U>
class generic_buffer {

	S thread_q_;

	bool stopping = false;
	
	std::mutex mx_;
	std::thread th_;
	std::condition_variable pushed_;

	void execute() {
		//one_shot_signal.wait();
		while (true)
		{
			S local_q_;
			{
				std::unique_lock<std::mutex> lock(mx_);
				pushed_.wait(lock, [this]() {return stopping || !is_empty(thread_q_); });
				if (stopping && is_empty(thread_q_)) break;
				std::swap(local_q_, thread_q_);
			}

			while (!is_empty(local_q_)) {
				callback(pull(local_q_));
				shift(local_q_);
				std::this_thread::sleep_for(std::chrono::seconds(2));
			}

		}
	}

	void stop() {
		stopping = true;
	}


protected:
	one_shot one_shot_signal;
	void createThread() {
		th_ = std::thread(&generic_buffer<T, S, U>::execute, this);
	}

public:

	generic_buffer() {}

	virtual bool is_empty(const S& container) const = 0;

	virtual U& pull(S& container) = 0;
	virtual void shift(S& container) = 0;

	virtual void callback(U& value) = 0;

	virtual void add(const T& value, S& container) = 0;

	void operator()(T val) {
		{
			std::unique_lock<std::mutex> lock(mx_);
			add(val, thread_q_);
		}
		pushed_.notify_one();
	}

	~generic_buffer() {
		stop();
		th_.join();
	}
};

#endif