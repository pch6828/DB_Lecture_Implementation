#ifndef ___KEY_VALUE___
#define ___KEY_VALUE___

template<class key_t, class value_t>
class Key_Value {
private:
	key_t key;
	value_t* value;
public:
	Key_Value(key_t key, value_t value):key(key) {
		this->value = new value(value);
	}

	void set_value(value_t value) {
		*this->value = value;
	}
};

#endif
