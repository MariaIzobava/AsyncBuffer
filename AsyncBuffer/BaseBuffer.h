#ifndef BASEBUFFER_H
#define BASEBUFFER_H

#include <functional>

class BaseBuffer {
public:

	virtual void callback() = 0;

	virtual void add(int) = 0;
	virtual void add() = 0;

	virtual bool is_triggered() const = 0;

	template <class Fn, class... Args>
	void custom_callback(Fn fn, Args&&... args) {
		fn(std::forward<Args>(args)...);
	}

};

#endif //BASEBUFFER_H