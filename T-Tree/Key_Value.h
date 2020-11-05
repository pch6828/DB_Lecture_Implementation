#ifndef ___KEY_VALUE___
#define ___KEY_VALUE___

#include<iostream>
using namespace std;

//pre-definition for some special field
#define NONE_KEY -1
#define NONE_VALUE NULL

//Class Definition for Key Value Pair
//Fields: 
//key, value pointer, bit for validation
//Methods:
//getter, setter for each field, custom operator
template<class key_t, class value_t>
class Key_Value {
private:
	bool valid;
	key_t key;
	value_t* value;
public:
	Key_Value(key_t key, value_t value):valid(true), key(key) {
		this->value = new value_t(value);
	}
	Key_Value() : valid(false), key(NONE_KEY), value(NULL) {}
	~Key_Value() {
		if (value) {
			delete this->value;
		}
	}
	
	Key_Value<key_t, value_t>& operator=(const Key_Value<key_t, value_t>& other);

	void set_value(value_t value);
	key_t get_key();
	value_t get_value();
	value_t* get_pointer();
	bool is_valid();
};

//Custom Operator for Assignment
template<class key_t, class value_t>
Key_Value<key_t, value_t>& Key_Value<key_t, value_t>::operator=(const Key_Value<key_t, value_t>& other) {
	if (other.valid) {
		this->key = other.key;
		this->value = new value_t(*other.value);
	}
	else {
		this->key = NONE_KEY;
		this->value = NONE_VALUE;
	}
	this->valid = other.valid;
	return *this;
}

//Setter for value field
template<class key_t, class value_t>
void Key_Value<key_t, value_t>::set_value(value_t value) {
	if (!this->value) {
		this->value = new value_t;
	}
	*this->value = value;
	this->valid = true;
}

//Getter for key field
template<class key_t, class value_t>
key_t Key_Value<key_t, value_t>::get_key() {
	return this->key;
};

//Getter for value field
//return actual value
template<class key_t, class value_t>
value_t Key_Value<key_t, value_t>::get_value() {
	return *this->value;
}

//Getter for value field
//return its pointer
template<class key_t, class value_t>
value_t* Key_Value<key_t, value_t>::get_pointer() {
	return this->value;
}

//Getter for validation bit field
template<class key_t, class value_t>
bool Key_Value<key_t, value_t>::is_valid() {
	return this->valid;
}
#endif
