#include "ThreadHive.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>

void do_work() {
    for (int i = 0; i < 200000000; ++i) {}
    // std::cout << "Done a unit of work\n";
}

void do_work_with_return(int &output) {
    std::srand(std::time(nullptr));
    int num = std::rand();
    for (int i = 0; i < 200000000; ++i) {}  // simulate work by running an empty loop
    // std::cout << "Done a unit of work\n";
    output = num;
}

void test_single_thread() {
    std::cout << "Single thread: start\n";
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 12; ++i) do_work();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Single thread: done in " << elapsed.count() << " seconds.\n";
    std::cout << "----------------------------------------\n";
}

void test_thread_pool() {
    std::cout << "Thread pool: start\n";
    auto start = std::chrono::high_resolution_clock::now();
    {
        ThreadHive pool(1);
        pool.resize(4);
        for (int i = 0; i < 12; ++i) pool.enqueue(do_work);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Thread pool: done in " << elapsed.count() << " seconds.\n";
    std::cout << "----------------------------------------\n";
}

void test_thread_pool_with_return() {
    std::cout << "Thread pool with return: start\n";
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> results(12);
    {
        ThreadHive pool(4);
        for (int i = 0; i < 12; ++i) pool.enqueue([&results, i](){ do_work_with_return(results[i]); });
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Thread pool with return: done in " << elapsed.count() << " seconds.\n";

    std::cout << "Results:\n";
    for (int i = 0; i < 12; ++i) std::cout << results[i] << " ";
    std::cout << "\n";
    std::cout << "----------------------------------------\n";
}

void test_wait_all() {
    std::cout << "Thread pool wait_all(): start\n";
    auto start = std::chrono::high_resolution_clock::now();
    ThreadHive pool(1);
    pool.resize(4);
    for (int i = 0; i < 12; ++i) pool.enqueue(do_work);
    pool.wait_all();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Thread pool wait_all(): done in " << elapsed.count() << " seconds.\n";
    std::cout << "----------------------------------------\n";
}

int main() {

    test_single_thread();
    test_thread_pool();
    test_thread_pool_with_return();
    test_wait_all();

    return 0;
}
