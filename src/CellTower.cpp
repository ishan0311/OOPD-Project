#include "../include/CellTower.h"

CellTower::CellTower(int towerId,
                     const char* loc,
                     Protocol* proto,
                     int totalBW)
    : id(towerId),
      protocol(proto),
      totalBWkHz(totalBW),
      channelCount(0) {
    copyString(loc, location, 32);
    setupChannels();
}

void CellTower::setupChannels() {
    if (!protocol) {
        throw SimException("Protocol is null in tower");
    }
    int cbw = protocol->getChannelBWkHz();
    if (cbw <= 0) {
        throw SimException("Invalid channel bandwidth");
    }
    channelCount = totalBWkHz / cbw;
    if (channelCount <= 0) {
        throw SimException("Calculated zero channels");
    }
    if (channelCount > MAX_CHANNELS) {
        throw SimException("Too many channels for fixed array");
    }
}

int CellTower::getMaxUsersBySpectrum() const {
    return channelCount * protocol->getUsersPerChannel();
}

bool CellTower::addUser(UserDevice* user) {
    int perChannelLimit = protocol->getUsersPerChannel();

    for (int i = 0; i < channelCount; ++i) {
        // only add if this channel hasn't reached logical limit
        if (channels[i].getSize() < perChannelLimit) {
            if (channels[i].add(user)) {
                user->setProtocol(protocol);
                user->setChannelIndex(i);
                return true;
            }
        }
    }
    return false; // all channels full by protocol limit
}

const StaticArray<UserDevice*, MAX_USERS_PER_CHANNEL>&
CellTower::getChannel(int idx) const {
    // assume idx valid
    return channels[idx];
}

int CellTower::getChannelCount() const {
    return channelCount;
}
