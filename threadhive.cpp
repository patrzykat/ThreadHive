#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <chrono>

class ThreadHive {
public:
    ThreadHive(size_t numThreads) {
        stop = false;
        pthread_mutex_init(&queue_mutex, nullptr);
        for (size_t i = 0; i < numThreads; ++i) {
            pthread_t thread;
            pthread_create(&thread, nullptr, &ThreadHive::perform_task, this);
            workers.push_back(thread);
        }
    }

    ~ThreadHive() {
        stop = true;
        for (pthread_t worker : workers)
            pthread_join(worker, nullptr);
        pthread_mutex_destroy(&queue_mutex);
    }

    void enqueue(std::function<void()> task) {
        pthread_mutex_lock(&queue_mutex);
        if (stop) {
            pthread_mutex_unlock(&queue_mutex);
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        tasks.push(std::move(task));
        pthread_mutex_unlock(&queue_mutex);
    }

private:
    std::vector<pthread_t> workers;
    std::queue<std::function<void()>> tasks;
    pthread_mutex_t queue_mutex;
    bool stop;

    static void* perform_task(void* arg) {
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
};

void do_work() {
    for (int i = 0; i < 500000000; ++i) {}  // simulate work by running an empty loop
    std::cout << "Done a unit of work\n";
}

int main() {
    std::cout << "Single thread: start\n";
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; ++i) do_work();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Single thread: done in " << elapsed.count() << " seconds.\n";
    
    std::cout << "Thread pool: start\n";
    // ThreadHive destructor will automatically be called when the object goes out of scope
    // Explicitly calling destructor caused double free error
    {
        ThreadHive pool(4);
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 10; ++i) pool.enqueue(do_work);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Thread pool: done in " << elapsed.count() << " seconds.\n";

    return 0;
}