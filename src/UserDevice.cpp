#include "../include/UserDevice.h"

UserDevice::UserDevice() : id(-1), traffic(TRAFFIC_DATA) {}

UserDevice::UserDevice(int i, TrafficType t) : id(i), traffic(t) {}

int UserDevice::getId() const {
    return id;
}

TrafficType UserDevice::getTrafficType() const {
    return traffic;
}
