#ifndef THREADHIVE_H
#define THREADHIVE_H

#include <queue>
#include <vector>
#include <pthread.h>
#include <functional>

class ThreadHive {
public:
    ThreadHive(size_t numThreads);
    ~ThreadHive();
    void enqueue(std::function<void()> task);
    void resize(size_t new_size);
    void wait_all();

private:
    std::vector<pthread_t> workers;
    std::queue<std::function<void()>> tasks;
    pthread_mutex_t queue_mutex;
    pthread_cond_t condition;
    size_t active_tasks;
    bool stop;

    static void* perform_task(void* arg);
};

#endif // THREADHIVE_H