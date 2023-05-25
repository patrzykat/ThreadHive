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
    active_tasks = 0;
    pthread_mutex_init(&queue_mutex, nullptr);
    pthread_cond_init(&condition, nullptr);
    resize(numThreads);
}

ThreadHive::~ThreadHive() {
    stop = true;
    for (pthread_t worker : workers)
        pthread_join(worker, nullptr);
    pthread_cond_destroy(&condition);
    pthread_mutex_destroy(&queue_mutex);
}

void ThreadHive::enqueue(std::function<void()> task) {
    pthread_mutex_lock(&queue_mutex);
    if (stop) {
        pthread_mutex_unlock(&queue_mutex);
        throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    tasks.push(std::move(task));
    active_tasks++;
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

void ThreadHive::wait_all() {
    pthread_mutex_lock(&queue_mutex);
    while (active_tasks > 0) {
        pthread_cond_wait(&condition, &queue_mutex);
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
            pool->active_tasks--;
            if (pool->active_tasks == 0) {
                pthread_cond_signal(&(pool->condition));
            }
        }
    }
}
