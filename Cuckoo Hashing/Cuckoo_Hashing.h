#ifndef __CUCKOO_HASHING__
#define __CUCKOO_HASHING__

#include "Key_Value.h"
#include <functional>
#include <random>

typedef unsigned long long llsize_t;

template<class key_t, class value_t, class hasher = hash<key_t>>
class Cuckoo_Hashmap {
private:
	std::random_device rd;
	size_t size[2];
	size_t hash_key_z[2];
	Key_Value<key_t, value_t>* hash_map[2];

	void set_hash_key_z(int table) {
		size_t key = rd();
		key += !(key % 2);
		hash_key_z[table] = key;
	}

	size_t get_idx(int table, key_t key) {
		llsize_t hashed_value = hasher(key);

		hashed_value *= hash_key_z[table];
		hashed_value %= (1ull << 32);
		hashed_value /= (1ull << (32 - size[table]));

		return hashed_value;
	}
	/*
	* TODO : implement these function
	* 
	void rehash(int table) {
		size_t prev_size = size[table]++;
		Key_Value* temp_table = hash_map[table];
		hash_map[table] = new Key_Value * [1 << size[table]];
		memset(hash_map[table], 0, 1 << size[table]);

		set_hash_key_z(table);
		for (int i = 0; i < prev_size; i++) {
			if (temp_table[prev_size]) {
				
			}
		}
	}

	void insert_or_assign(Key_Value<key_t, value_t>* k_v) {
		key_t key;
		int table = 0, iteration = 0;
		while (true) {
			iteration++;
			key = k_v->get_key();
			size_t idx = get_idx(table, key);

			swap(k_v, hash_map[table][idx]);
			if (!k_v || iteration==0) {
				break;
			}
			else {

			}
		}
	}
	*/
public:

};


#endif