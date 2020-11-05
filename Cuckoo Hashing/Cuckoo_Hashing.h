#ifndef __CUCKOO_HASHING__
#define __CUCKOO_HASHING__

#include "Key_Value.h"
#include <functional>
#include <random>
#include <iostream>

typedef unsigned long long llsize_t;

//pre-definition for rehash condition value
constexpr auto MAX_ITERATION = 10;

//Class Definition for Key Value Pair
//Fields: 
//random device for hashing key, two hash table, size, hashing key, initial hashing function
//Methods:
//Hashing function, Search, Insertion, Deletion, Custom operator, Printing (for debugging)
template<class key_t, class value_t, class hasher_t = std::hash<key_t>>
class Cuckoo_Hashmap {
private:
	std::random_device rd;
	size_t size[2];
	size_t hash_key_z[2];
	Key_Value<key_t, value_t>** hash_map[2];
	hasher_t hasher;

	//Setter for Random Hasing Key
	//set hashing key for given table(0 or 1) to random positive odd number
	void set_hash_key_z(int table) {
		size_t key = rd();
		key += !(key % 2);
		hash_key_z[table] = key;
	}

	//Caculate Hashed Index
	//Using multiplicative hashing function to calculate index
	size_t get_idx(int table, key_t key) {	
		llsize_t hashed_value = hasher(key);

		hashed_value *= hash_key_z[table];
		hashed_value %= (1ull << 32);
		hashed_value /= (1ull << (32 - size[table]));

		return hashed_value;
	}

	//Helper Function for Insertion
	//if there is empty slot for given key or there is already corresponding key, just place record
	//else, pull out already existing record and place new record, then place picked record to other table
	//
	//after some iteration, if insertion process is not completed, rehash entire table.
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
			else if (k_v->get_key() == hash_map[table][idx]->get_key()) {
				delete k_v;
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

	//Rehashing Operation
	//allocate new hash table that has twice the previous table size
	//set new random hashing key for two new table
	//hash all records in previous table into new table
	//deallocate previous table
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

	//Single Key Insertion API
	//if there are already corresponding key, do nothing
	//else, insert given record
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

	//Single Key Search API
	//if there are corresponding key, return its value's reference.
	//else, allocate new record with given key and default value, and return its value's reference
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

	//Custom Operator []
	//same action with Cuckoo_Hashtable::find
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

	//Single Key Deletion
	//if there is not corresponding key, do nothing
	//else, delete that record and set its slot empty.
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

	//For Debug Printing Function
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