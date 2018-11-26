#ifndef BASE_NOTIFIER_H
#define BASE_NOTIFIER_H

template < class T>
class base_notifier {

private:
	void (*_callback)(T);

protected:
	bool stopping;

public:
	base_notifier(void (*callback_function)(T)) : _callback(callback_function) {}

	void callback(T val) {
		_callback(val);
	}

	void stop() {
		stopping = true;
	}

};

#endif //BASEBUFFER_H