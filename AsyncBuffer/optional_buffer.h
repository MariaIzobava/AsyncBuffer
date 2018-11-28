#ifndef OPTIONAL_BUFFER_H
#define OPTIONAL_BUFFER_H

#include "generic_buffer.h"
#include <optional>
#include <cassert>

template  <class T>
class optional_buffer : public generic_buffer<T, std::optional<T>, T> {

	void(*_callback)(T);
	std::thread th_;
public:
	optional_buffer(void(*callback)(T)) : _callback(callback), generic_buffer<T, std::queue<T>, T>() {
		generic_buffer<T, std::queue<T>, T>::one_shot_signal.signal();

	}

	bool is_empty(const std::queue<T>& container) const {
		return container.empty();
	}

	T& pull(std::queue<T>& container) {
		assert(!container.empty());
		return container.front();
	}
	void shift(std::queue<T>& container) {
		assert(!container.empty());
		container.pop();
	}
	void callback(T& value) {
		_callback(value);
	}

	void add(const T& value, std::queue<T>& container) {
		container.push(value);
	}

	~optional_buffer() {}
};

#endif