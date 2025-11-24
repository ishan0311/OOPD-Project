#pragma once

#include "UserDevice.h"

class Protocol {
public:
    virtual const char* name() const = 0;
    virtual int channelBandwidthKHz() const = 0;
    virtual int maxUsersPerChannel() const = 0;
    virtual int maxAntennas() const = 0;
    virtual int messagesPerUser(TrafficType t) const = 0;
    virtual ~Protocol() {}
};

// 2G: TDMA, 16 users/200kHz, 5 data msgs, 15 voice msgs
class Protocol2G : public Protocol {
public:
    virtual const char* name() const { return "2G"; }
    virtual int channelBandwidthKHz() const { return 200; }
    virtual int maxUsersPerChannel() const { return 16; }
    virtual int maxAntennas() const { return 1; }
    virtual int messagesPerUser(TrafficType t) const {
        if (t == TRAFFIC_DATA)  return 5;
        if (t == TRAFFIC_VOICE) return 15;
        return 20; // both
    }
};

// 3G: CDMA, 32 users/200kHz, 10 messages/user
class Protocol3G : public Protocol {
public:
    virtual const char* name() const { return "3G"; }
    virtual int channelBandwidthKHz() const { return 200; }
    virtual int maxUsersPerChannel() const { return 32; }
    virtual int maxAntennas() const { return 1; }
    virtual int messagesPerUser(TrafficType /*t*/) const {
        return 10;
    }
};

// 4G: OFDM, 30 users/10kHz, 10 messages/user, up to 4 antennas
class Protocol4G : public Protocol {
public:
    virtual const char* name() const { return "4G"; }
    virtual int channelBandwidthKHz() const { return 10; }
    virtual int maxUsersPerChannel() const { return 30; }
    virtual int maxAntennas() const { return 4; }
    virtual int messagesPerUser(TrafficType /*t*/) const {
        return 10;
    }
};

// 5G: 30 users per 1MHz (high band), 10 messages/user, up to 16 antennas
class Protocol5G : public Protocol {
public:
    virtual const char* name() const { return "5G"; }
    virtual int channelBandwidthKHz() const { return 1000; } // 1 MHz
    virtual int maxUsersPerChannel() const { return 30; }
    virtual int maxAntennas() const { return 16; }
    virtual int messagesPerUser(TrafficType /*t*/) const {
        return 10;
    }
};
