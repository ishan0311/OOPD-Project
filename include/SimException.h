#pragma once

class SimException {
public:
    const char* msg;
    SimException(const char* m) : msg(m) {}
};
