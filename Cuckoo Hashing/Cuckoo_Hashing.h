#ifndef __CUCKOO_HASHING__
#define __CUCKOO_HASHING__

#include "Key_Value.h"
#include <functional>
#include <random>
#include <iostream>

typedef unsigned long long llsize_t;
constexpr auto MAX_ITERATION = 10;

template<class key_t, class value_t, class hasher_t = hash<key_t>>
class Cuckoo_Hashmap {
private:
	std::random_device rd;
	size_t size[2];
	size_t hash_key_z[2];
	Key_Value<key_t, value_t>** hash_map[2];
	hasher_t hasher;

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

	void insert_or_assign(Key_Value<key_t, value_t>* k_v) {
		key_t key;
		int table = 0, iteration = 0;
		while (true) {
			iteration++;
			key = k_v->get_key();
			size_t idx = get_idx(table, key);

			swap(k_v, hash_map[table][idx]);
			if (!k_v) {
				return;
			}
			else if (iteration == MAX_ITERATION) {
				rehash();
				insert_or_assign(k_v);
				return;
			}
			else {
				table = !table;
			}
		}
	}

	void rehash() {
		size_t prev_size0 = size[0]++;
		size_t prev_size1 = size[1]++;
		size_t table_size0 = 1 << prev_size0;
		size_t table_size1 = 1 << prev_size1;
		
		Key_Value<key_t, value_t>** temp_table0 = hash_map[0];
		Key_Value<key_t, value_t>** temp_table1 = hash_map[1];

		hash_map[0] = new Key_Value<key_t, value_t> * [1 << size[0]];
		hash_map[1] = new Key_Value<key_t, value_t> * [1 << size[1]];
		memset(hash_map[0], 0, (1 << size[0]) * sizeof(Key_Value<key_t, value_t>*));
		memset(hash_map[1], 0, (1 << size[1]) * sizeof(Key_Value<key_t, value_t>*));

		set_hash_key_z(0);
		set_hash_key_z(1);
		
		for (int i = 0; i < table_size0; i++) {
			if (temp_table0[i]) {
				insert_or_assign(temp_table0[i]);
			}
		}
		for (int i = 0; i < table_size1; i++) {
			if (temp_table1[i]) {
				insert_or_assign(temp_table1[i]);
			}
		}
		delete[] temp_table0;
		delete[] temp_table1;
	}

public:
	Cuckoo_Hashmap() {
		size[0] = size[1] = 0;
		set_hash_key_z(0);
		set_hash_key_z(1);

		hash_map[0] = new Key_Value<key_t, value_t> * [1];
		hash_map[1] = new Key_Value<key_t, value_t> * [1];
		hash_map[0][0] = hash_map[1][0] = NULL;
	}

	~Cuckoo_Hashmap() {
		size_t table_size;
		table_size = (1ull << size[0]);
		for (size_t i = 0; i < table_size; i++) {
			if (hash_map[0][i]) {
				delete hash_map[0][i];
			}
		}
		table_size = (1ull << size[1]);
		for (size_t i = 0; i < table_size; i++) {
			if (hash_map[1][i]) {
				delete hash_map[1][i];
			}
		}

		delete[] hash_map[0];
		delete[] hash_map[1];
	}

	void insert(key_t key, value_t value) {
		Key_Value<key_t, value_t>* k_v0, * k_v1;

		k_v0 = hash_map[0][get_idx(0, key)];
		k_v1 = hash_map[1][get_idx(1, key)];

		if (k_v0 && k_v0->get_key() == key) {
			return;
		}
		else if (k_v1 && k_v1->get_key() == key) {
			return;
		}

		Key_Value<key_t, value_t>* k_v = new Key_Value<key_t, value_t>(key, value);
		insert_or_assign(k_v);
	}

	value_t& find(key_t key) {
		Key_Value<key_t, value_t>* k_v0, * k_v1;

		k_v0 = hash_map[0][get_idx(0, key)];
		k_v1 = hash_map[1][get_idx(1, key)];

		if (k_v0 && k_v0->get_key() == key) {
			return *(k_v0->get_pointer());
		}
		else if (k_v1 && k_v1->get_key() == key) {
			return *(k_v1->get_pointer());
		}
		insert_or_assign(new Key_Value<key_t, value_t>(key));

		k_v0 = hash_map[0][get_idx(0, key)];
		k_v1 = hash_map[1][get_idx(1, key)];

		if (k_v0 && k_v0->get_key() == key) {
			return *(k_v0->get_pointer());
		}
		else if (k_v1 && k_v1->get_key() == key) {
			return *(k_v1->get_pointer());
		}
	}

	value_t& operator[](key_t key) {
		Key_Value<key_t, value_t>* k_v0, * k_v1;

		k_v0 = hash_map[0][get_idx(0, key)];
		k_v1 = hash_map[1][get_idx(1, key)];

		if (k_v0 && k_v0->get_key() == key) {
			return *(k_v0->get_pointer());
		}
		else if (k_v1 && k_v1->get_key() == key) {
			return *(k_v1->get_pointer());
		}
		insert_or_assign(new Key_Value<key_t, value_t>(key));

		k_v0 = hash_map[0][get_idx(0, key)];
		k_v1 = hash_map[1][get_idx(1, key)];

		if (k_v0 && k_v0->get_key() == key) {
			return *(k_v0->get_pointer());
		}
		else if (k_v1 && k_v1->get_key() == key) {
			return *(k_v1->get_pointer());
		}
	}

	void erase(key_t key) {
		Key_Value<key_t, value_t>* k_v0, * k_v1;

		k_v0 = hash_map[0][get_idx(0, key)];
		k_v1 = hash_map[1][get_idx(1, key)];

		if (k_v0 && k_v0->get_key() == key) {
			delete k_v0;
			hash_map[0][get_idx(0, key)] = NULL;
		}
		else if (k_v1 && k_v1->get_key() == key) {
			delete k_v1;
			hash_map[1][get_idx(1, key)] = NULL;
		}
	}

	void print_table() {
		size_t table_size;
		std::cout << "table 0" << std::endl;
		table_size = (1ull << size[0]);
		for (size_t i = 0; i < table_size; i++) {
			if (hash_map[0][i]) {
				std::cout << hash_map[0][i]->get_key() << " ";
			}
			else {
				std::cout << "null ";
			}
		}
		std::cout << std::endl;
		std::cout << "table 1" << std::endl;
		table_size = (1ull << size[1]);
		for (size_t i = 0; i < table_size; i++) {
			if (hash_map[1][i]) {
				std::cout << hash_map[1][i]->get_key() << " ";
			}
			else {
				std::cout << "null ";
			}
		}
		std::cout << std::endl;
	}
};


#endif