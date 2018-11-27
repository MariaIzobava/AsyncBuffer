#ifndef LOGBUFFER_H
#define LOGBUFFER_H

#include <queue>
#include <cassert>
#include "BaseBuffer.h"

template <class Fn>
class LogBuffer :public BaseBuffer {
public:
	Fn fn_;
	bool is_triggered_ = false;
public:
	LogBuffer(Fn fn) :fn_(fn) {}

	void add(int) {}

	void add() {
		is_triggered_ = true;
	}

	bool is_triggered() const {
		return is_triggered_;
	}

	void callback() {
		custom_callback(fn_);
		is_triggered_ = false;
	}

	~LogBuffer() {}
};

#endif