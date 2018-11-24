#ifndef BUFFERNOTIFIER_H
#define BUFFERNOTIFIER_H

#include "LogBuffer.h"
#include "CountBuffer.h"
#include "QueueBuffer.h"
#include <thread>
#include <algorithm>
#include <mutex>

class BufferNotifier {
private:
	std::vector<std::thread> threads_;
	
	BaseBuffer* qbuffer_;
	BaseBuffer* lbuffer_;
	BaseBuffer* cbuffer_;

	size_t n_threads_;

	void add_thread(BaseBuffer* buffer) {
		threads_.push_back(std::thread(&BufferNotifier::receiver_, this, buffer));
	}

	std::mutex g_value_mx;
	std::condition_variable g_cv_produced;
	std::condition_variable g_cv_consumed;
	bool g_stopping = false;

	void receiver_(BaseBuffer* buf)
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

	void wait_for_consuming() {
		{
			std::unique_lock<std::mutex> lock(g_value_mx);
			g_cv_consumed.wait(lock, [&]() { return !qbuffer_->is_triggered() && !cbuffer_->is_triggered() && !lbuffer_->is_triggered(); });
		}

		{
			std::lock_guard<std::mutex> lock(g_value_mx);
			g_stopping = true;
		}

		g_cv_produced.notify_all();
	}

public:

	//BufferNotifier(size_t n_threads): n_threads_(n_threads) {}

	template <class T, class Fn>
	void add_queue_buffer(Fn fn) {
		qbuffer_ = new QueueBuffer<T, Fn>(fn);
		add_thread(qbuffer_);
	}
	template <class Fn>
	void add_count_buffer(Fn fn) {
		cbuffer_ = new CountBuffer<Fn>(fn);
		add_thread(cbuffer_);
	}

	template <class Fn> 
	void add_log_buffer(Fn fn) {
		lbuffer_ =  new LogBuffer<Fn>(fn);
		add_thread(lbuffer_);
	}

	template <class T>
	void add_value(T val) {
		{
			std::lock_guard<std::mutex> lock(g_value_mx);
			qbuffer_->add(val);
			cbuffer_->add();
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		g_cv_produced.notify_all();
	}

	void add_log_message() {
		{
			std::lock_guard<std::mutex> lock(g_value_mx);
			lbuffer_->add();
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		g_cv_produced.notify_all();
	}


	~BufferNotifier() {
		wait_for_consuming();
		for (auto it = threads_.begin(); it != threads_.end(); it++)
			it->join();
	}
};

#endif