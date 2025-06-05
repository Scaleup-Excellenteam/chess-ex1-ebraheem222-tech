#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <list>
#include <stdexcept>

// Exception when priority queue is empty and poll() called
class EmptyQueueException : public std::exception {
public:
    const char* what() const noexcept override {
        return "PriorityQueue is empty";
    }
};

// Exception when comparator cannot compare
class ComparisonException : public std::exception  {
public:
    const char* what() const noexcept override {
        return "Comparison failed";
    }
};


template<typename T, typename Comparator>
class PriorityQueue {
private:
    std::list<T> data;
    Comparator comp;
    size_t maxSize;

public:
    PriorityQueue(size_t maxSize = 5) : maxSize(maxSize) {}

    void push(const T& value) {
        if (data.empty()) {
            data.push_back(value);
        } else {
            bool inserted = false;
            for (auto it = data.begin(); it != data.end(); ++it) {
                int cmp;
                try {
                    cmp = comp(value, *it);
                } catch (...) {
                    throw ComparisonException();
                }
                if (cmp > 0) {
                    data.insert(it, value);
                    inserted = true;
                    break;
                }
            }
            if (!inserted) {
                data.push_back(value);
            }
        }
        if (data.size() > maxSize) {
            data.pop_back(); 
        }
    }

    T poll() {
        if (data.empty()) throw EmptyQueueException();
        T front = data.front();
        data.pop_front();
        return front;
    }

    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }

    typename std::list<T>::const_iterator begin() const { return data.begin(); }
    typename std::list<T>::const_iterator end() const { return data.end(); }
};

#endif // PRIORITYQUEUE_H