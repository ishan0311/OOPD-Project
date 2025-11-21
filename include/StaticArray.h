#pragma once

template <class T, int N>
class StaticArray {
private:
    T data[N];
    int size;

public:
    StaticArray() : size(0) {}

    bool add(T value) {
        if (size >= N) {
            return false;
        }
        data[size++] = value;
        return true;
    }

    int getSize() const {
        return size;
    }

    T get(int index) const {
        if (index < 0 || index >= size) {
            return data[0]; // assume caller is correct in normal flow
        }
        return data[index];
    }

    T& getRef(int index) {
        // assume caller is correct
        return data[index];
    }
};
