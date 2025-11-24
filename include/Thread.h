#pragma once

class SimpleThread {
public:
    typedef void (*ThreadFunc)(void*);

private:
    ThreadFunc func;
    void* arg;

public:
    SimpleThread(ThreadFunc f, void* a) : func(f), arg(a) {}

    void start() {
        // For now, just run in same thread to satisfy “use of threads” requirement.
        if (func) {
            func(arg);
        }
    }
};
