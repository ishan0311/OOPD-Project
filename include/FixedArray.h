#pragma once

template <typename T, int N>
class FixedArray {
private:
    T data[N];
    int count;
public:
    FixedArray() : count(0) {}

    int size() const { return count; }
    int capacity() const { return N; }

    void clear() { count = 0; }

    bool push_back(const T& value) {
        if (count >= N) return false;
        data[count] = value;
        ++count;
        return true;
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }
};
