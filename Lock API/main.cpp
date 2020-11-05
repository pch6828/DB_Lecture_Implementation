#include <iostream>
#include <thread>
#include <chrono>
#include "Lock_Table.h"

using namespace std;

bool ready = false;
mutex mtx;
condition_variable cv;
Lock_Table<int>lock_table;


void thread1(int thread_idx){
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) cv.wait(lck);
    Lock* lock = lock_table.acquire_lock(1);

    cout<<thread_idx << " 1" <<endl;
    this_thread::sleep_for(chrono::seconds(5));
    lock_table.release_lock(lock);
}

void thread2(int thread_idx){
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) cv.wait(lck);    
    Lock* lock = lock_table.acquire_lock(2);

    cout<<thread_idx << " 2" <<endl;
    this_thread::sleep_for(chrono::seconds(5));
    lock_table.release_lock(lock);
}

void thread3(int thread_idx){
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) cv.wait(lck);
    Lock* lock = lock_table.acquire_lock(1);

    cout<<thread_idx << " 1" <<endl;

    lock_table.release_lock(lock);
}

void thread4(int thread_idx){
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) cv.wait(lck);
    Lock* lock = lock_table.acquire_lock(2);

    cout<<thread_idx << " 2" <<endl;

    lock_table.release_lock(lock);
}

void go() {
  std::unique_lock<std::mutex> lck(mtx);
  ready = true;
  cv.notify_all();
}


int main(){
    ready = true;

    thread thread_1(thread1, 1);
    thread thread_2(thread2, 2);
    thread thread_3(thread3, 3);
    thread thread_4(thread4, 4);

    thread_1.join();
    thread_2.join();
    thread_3.join();
    thread_4.join();
}