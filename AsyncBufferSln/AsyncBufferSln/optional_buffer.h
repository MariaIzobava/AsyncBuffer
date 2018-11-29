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
	optional_buffer(void(*callback)(T)) : _callback(callback), generic_buffer<T, std::optional<T>, T>() {
		//generic_buffer<T, std::optional<T>, T>::one_shot_signal.signal();
		generic_buffer<T, std::optional<T>, T>::createThread();
	}

	bool is_empty(const std::optional<T>& container) const {
		return !container.has_value();
	}

	T& pull(std::optional<T>& container) {
		assert(container.has_value());
		return container.value();
	}
	void shift(std::optional<T>& container) {
		assert(container.has_value());
		container.reset();
	}
	void callback(T& value) {
		_callback(value);
	}

	void add(const T& value, std::optional<T>& container) {
		container = value;
	}

	~optional_buffer() {}
};

#endif