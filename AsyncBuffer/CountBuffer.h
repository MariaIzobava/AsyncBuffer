#include <queue>
#include <cassert>
#include "BaseBuffer.h"

template <class Fn>
class CountBuffer :public BaseBuffer<Fn> {
private:
	int count_ = 0;
	bool is_triggered_ = false;
public:
	CountBuffer(Fn fn) :BaseBuffer<Fn>(fn) {

	}
	void add() {
		count_++;
		is_triggered_ = true;
		std::cerr << "BBB!\n";
	}

	bool is_triggered() const {
		std::cerr << is_triggered_ << ' ' << "AAA!\n";
		return is_triggered_;
	}

	void callback() {
		BaseBuffer<Fn>::custom_callback(count_);
		is_triggered_ = false;
	}

	~CountBuffer() {}
};