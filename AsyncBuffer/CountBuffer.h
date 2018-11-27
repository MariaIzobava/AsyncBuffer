#ifndef COUNTBUFFER_H
#define COUNTBUFFER_H

#include <queue>
#include "BaseBuffer.h"

template <class Fn>
class CountBuffer : public BaseBuffer {
private:
	Fn fn_;
	int count_ = 0;
	bool is_triggered_ = false;
public:
	CountBuffer(Fn fn) : fn_(fn) {}
	void add(int) {}
	void add() {
		count_++;
		is_triggered_ = true;
	}

	bool is_triggered() const {
		return is_triggered_;
	}

	void callback() {
		custom_callback(fn_, count_);
		is_triggered_ = false;
	}

	~CountBuffer() {}
};

#endif