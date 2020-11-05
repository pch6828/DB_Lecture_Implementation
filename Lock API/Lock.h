#ifndef __LOCK_H__
#define __LOCK_H__

#include <condition_variable>
#include <mutex>

class Lock{
    private:
        std::condition_variable cond;
        Lock* prev, *next;
    public:
        Lock(){
            prev = next = nullptr;
        }
    friend class Lock_List;
};

class Lock_List{
    private:
        Lock* head, *tail;
        std::mutex latch;
    public:
        Lock_List(){
            head = new Lock();
            tail = new Lock();
            head->next = tail;
            tail->prev = head;
        }

        void add_lock(Lock* lock){
            Lock* prev = tail->prev;
            prev->next = lock;
            tail->prev = lock;

            lock->prev = prev;
            lock->next = tail;
        }

        void grant_or_sleep(Lock* lock){
            std::unique_lock<std::mutex>unique_latch(latch);
            while(lock->prev != head){
                lock->prev->cond.wait(unique_latch);
            }
        }

        void wakeup_next(Lock* lock){
            Lock* prev = lock->prev;
            Lock* next = lock->next;

            prev->next = next;
            next->prev = prev;

            lock->cond.notify_one();
            delete lock;
        }
};

#endif
