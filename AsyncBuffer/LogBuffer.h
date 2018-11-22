#include <queue>
#include <cassert>
#include "BaseBuffer.h"

template <class Fn>
class LogBuffer :public BaseBuffer<Fn> {
public:
	bool is_triggered_ = false;
public:
	LogBuffer(Fn fn) :BaseBuffer<Fn>(fn) {

	}

	void add() {
		//std::cout << "EEE\n";
		is_triggered_ = true;
	}

	bool is_triggered() const {
		return is_triggered_;
	}

	void callback() {
		BaseBuffer<Fn>::custom_callback();
		is_triggered_ = false;
	}

	~LogBuffer() {}
};