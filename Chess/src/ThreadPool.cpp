#include "../include/ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads)
    : stopFlag(false)
{
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerLoop, this);
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

void ThreadPool::shutdown() {
    stopFlag.store(true);
    tasksCv.notify_all();
    for (auto &w : workers) {
        if (w.joinable()) {
            w.join();
        }
    }
    workers.clear();
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(tasksMutex);
        tasks.push(std::move(task));
    }
    tasksCv.notify_one();
}

void ThreadPool::workerLoop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(tasksMutex);
            tasksCv.wait(lock, [this]() {
                return stopFlag.load() || !tasks.empty();
            });
            if (stopFlag.load() && tasks.empty()) {
                return;
            }
            job = std::move(tasks.front());
            tasks.pop();
        }
        job();
    }
}