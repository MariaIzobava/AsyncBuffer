#include <queue>
#include <cassert>
#include "BaseBuffer.h"

template <class T, class Fn> 
class QueueBuffer: public BaseBuffer<Fn> {
private:
	std::queue<T> q_;
public:
	QueueBuffer(Fn fn):BaseBuffer<Fn>(fn) {
		
	}
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
		BaseBuffer<Fn>::custom_callback(current_element);
	}

	~QueueBuffer() {}
};