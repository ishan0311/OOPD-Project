#pragma once

enum TrafficType {
    TRAFFIC_DATA  = 1,
    TRAFFIC_VOICE = 2,
    TRAFFIC_BOTH  = 3
};

class UserDevice {
private:
    int id;
    TrafficType traffic;
public:
    UserDevice();
    UserDevice(int i, TrafficType t);

    int getId() const;
    TrafficType getTrafficType() const;
};
