#ifndef QUEUEBUFFER_H
#define QUEUEBUFFER_H

#include <queue>
#include <cassert>
#include "BaseBuffer.h"

template <class T, class Fn> 
class QueueBuffer: public BaseBuffer {
private:
	Fn fn_;
	std::queue<T> q_;
public:
	QueueBuffer(Fn fn):fn_(fn) {}

	void add() {}

	void add(T value) {
		q_.push(value);
	}

	bool is_empty() const {
		return q_.empty();
	}

	bool is_triggered() const {
		return !is_empty();
	}

	void callback() {
		assert(!q_.empty());
		T current_element = q_.front();
		q_.pop();
		custom_callback(fn_, current_element);
	}

	~QueueBuffer() {}
};

#endif