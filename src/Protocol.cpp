#include "../include/Protocol.h"

// Base Protocol implementation

Protocol::Protocol(const char* nm,
                   int upc,
                   int cbw,
                   int msgPerUser,
                   int overhead)
    : usersPerChannel(upc),
      channelBWkHz(cbw),
      messagesPerUser(msgPerUser),
      overheadPer100(overhead) {
    copyString(nm, name, 16);
}

const char* Protocol::getName() const {
    return name;
}

int Protocol::getUsersPerChannel() const {
    return usersPerChannel;
}

int Protocol::getChannelBWkHz() const {
    return channelBWkHz;
}

int Protocol::getMessagesPerUser() const {
    return messagesPerUser;
}

int Protocol::getOverheadPer100Messages() const {
    return overheadPer100;
}

// NEW: setter so we can take overhead from user
void Protocol::setOverheadPer100Messages(int ov) {
    if (ov < 0) {
        overheadPer100 = 0;
    } else {
        overheadPer100 = ov;
    }
}

int Protocol::computeMaxUsersBySpectrum(int totalBWkHz) const {
    if (channelBWkHz <= 0) {
        throw SimException("Channel bandwidth must be positive");
    }
    int channels = totalBWkHz / channelBWkHz;
    if (channels <= 0) {
        throw SimException("Not enough bandwidth for one channel");
    }
    return channels * usersPerChannel;
}

// Derived protocols

// 2G: TDMA, 16 users / 200 kHz, default 20 messages/user, 20% overhead
Protocol2G::Protocol2G()
    : Protocol("2G", 16, 200, 20, 20) {}

// 3G: CDMA, 32 users / 200 kHz, 10 messages/user, 15% overhead
Protocol3G::Protocol3G()
    : Protocol("3G", 32, 200, 10, 15) {}

// 4G: OFDM, 30 users / 10 kHz, 10 messages/user, 10% overhead
Protocol4G::Protocol4G()
    : Protocol("4G", 30, 10, 10, 10) {}

// 5G: 30 users / 1 MHz, 10 MHz block, 10 messages/user, 8% overhead
Protocol5G::Protocol5G()
    : Protocol("5G", 30, 1000, 10, 8) {}
