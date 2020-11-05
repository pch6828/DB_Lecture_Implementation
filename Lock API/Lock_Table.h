#ifndef __LOCK_TABLE_H__
#define __LOCK_TABLE_H__

#include <unordered_map>
#include <functional>
#include "Lock.h"

template<class key_t, class hasher = std::hash<key_t>>
class Lock_Table{
    private:
        std::mutex lock_table_latch;
        std::unordered_map<key_t, Lock_List, hasher>table;
    public:
        Lock* acquire_lock(key_t key){
            lock_table_latch.lock();

            Lock* lock = new Lock();
            table[key].add_lock(lock);

            lock_table_latch.unlock();
            table[key].grant_or_sleep(lock);
            return lock;
        }
        int release_lock(Lock* lock){
            lock_table_latch.lock();
            
            lock_table_latch.unlock();

            return 0;
        }
};

#endif