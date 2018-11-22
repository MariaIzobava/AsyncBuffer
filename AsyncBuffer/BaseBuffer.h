#ifndef BASEBUFFER_H
#define BASEBUFFER_H

#include <functional>

template <class Fn>
class BaseBuffer {
private:
	Fn fn_;
public:

	BaseBuffer(Fn fn):fn_(fn) {}

	virtual void callback() = 0;

	virtual bool is_triggered() const = 0;

protected:
	template <class... Args>
	void custom_callback(Args&&... args) {
		fn_(std::forward<Args>(args)...);
	}

	

	~BaseBuffer() {}
};

#endif //BASEBUFFER_H