#include "ThreadHive.h"
#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <chrono>
#include <cstdlib>
#include <ctime>


ThreadHive::ThreadHive(size_t numThreads) {
    stop = false;
    pthread_mutex_init(&queue_mutex, nullptr);
    resize(numThreads);
}

ThreadHive::~ThreadHive() {
    stop = true;
    for (pthread_t worker : workers)
        pthread_join(worker, nullptr);
    pthread_mutex_destroy(&queue_mutex);
}

void ThreadHive::enqueue(std::function<void()> task) {
    pthread_mutex_lock(&queue_mutex);
    if (stop) {
        pthread_mutex_unlock(&queue_mutex);
        throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    tasks.push(std::move(task));
    pthread_mutex_unlock(&queue_mutex);
}

void ThreadHive::resize(size_t new_size) {
    if (new_size == workers.size()) return;

    pthread_mutex_lock(&queue_mutex);
    if (new_size > workers.size()) {
        for (size_t i = workers.size(); i < new_size; ++i) {
            pthread_t thread;
            pthread_create(&thread, nullptr, &ThreadHive::perform_task, this);
            workers.push_back(thread);
        }
    } else {
        stop = true;
        for (size_t i = new_size; i < workers.size(); ++i) {
            pthread_join(workers[i], nullptr);
        }
        workers.resize(new_size);
        stop = false;
    }
    pthread_mutex_unlock(&queue_mutex);
}

void* ThreadHive::perform_task(void* arg) {
    ThreadHive* pool = (ThreadHive*) arg;
    while (true) {
        std::function<void()> task;
        pthread_mutex_lock(&(pool->queue_mutex));
        if (pool->stop && pool->tasks.empty()) {
            pthread_mutex_unlock(&(pool->queue_mutex));
            return nullptr;
        }
        if(!pool->tasks.empty()){
            task = std::move(pool->tasks.front());
            pool->tasks.pop();
        }
        pthread_mutex_unlock(&(pool->queue_mutex));
        if(task) {
            task();
        }
    }
}

// void do_work() {
//     for (int i = 0; i < 200000000; ++i) {}
//     // std::cout << "Done a unit of work\n";
// }

// void do_work_with_return(int &output) {
//     std::srand(std::time(nullptr));
//     int num = std::rand();
//     for (int i = 0; i < 200000000; ++i) {}  // simulate work by running an empty loop
//     // std::cout << "Done a unit of work\n";
//     output = num;
// }

// void test_single_thread() {
//     std::cout << "Single thread: start\n";
//     auto start = std::chrono::high_resolution_clock::now();
//     for (int i = 0; i < 12; ++i) do_work();
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> elapsed = end - start;
//     std::cout << "Single thread: done in " << elapsed.count() << " seconds.\n";
//     std::cout << "----------------------------------------\n";
// }

// void test_thread_pool() {
//     std::cout << "Thread pool: start\n";
//     auto start = std::chrono::high_resolution_clock::now();
//     // ThreadHive destructor will automatically be called when the object goes out of scope
//     // Explicitly calling destructor caused double free error
//     {
//         ThreadHive pool(1);
//         pool.resize(4);
//         for (int i = 0; i < 12; ++i) pool.enqueue(do_work);
//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> elapsed = end - start;
//     std::cout << "Thread pool: done in " << elapsed.count() << " seconds.\n";
//     std::cout << "----------------------------------------\n";
// }

// void test_thread_pool_with_return() {
//     std::cout << "Thread pool with return: start\n";
//     auto start = std::chrono::high_resolution_clock::now();
//     std::vector<int> results(12);
//     // ThreadHive destructor will automatically be called when the object goes out of scope
//     // Explicitly calling destructor caused double free error
//     {
//         ThreadHive pool(4);
//         for (int i = 0; i < 12; ++i) pool.enqueue([&results, i](){ do_work_with_return(results[i]); });
//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> elapsed = end - start;
//     std::cout << "Thread pool with return: done in " << elapsed.count() << " seconds.\n";

//     std::cout << "Results:\n";
//     for (int i = 0; i < 12; ++i) std::cout << results[i] << " ";
//     std::cout << "\n";
//     std::cout << "----------------------------------------\n";
// }

// int main() {

//     test_single_thread();
//     test_thread_pool();
//     test_thread_pool_with_return();

//     return 0;
// }