#ifndef BASE_NOTIFIER_H
#define BASE_NOTIFIER_H

#include <functional>

template < class T>
class base_notifier {

private:
	void (*_callback)(T);

public:
	base_notifier(void (*callback_function)(T)) : _callback(callback_function) {}

	void callback(T val) {
		_callback(val);
	}

};

#endif //BASEBUFFER_H