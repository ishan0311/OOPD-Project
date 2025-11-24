#pragma once

class SimulationException {
private:
    const char* msg;
public:
    SimulationException(const char* m) : msg(m) {}
    const char* what() const { return msg; }
};
