#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads);
    ~ThreadPool();
    void enqueue(std::function<void()> task);
    void shutdown();

private:
    void workerLoop();
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex tasksMutex;
    std::condition_variable tasksCv;
    std::atomic<bool> stopFlag;
};

#endif // THREADPOOL_H