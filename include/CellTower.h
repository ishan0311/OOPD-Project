#pragma once

#include "Protocol.h"
#include "SimException.h"
#include "StaticArray.h"
#include "UserDevice.h"

#define MAX_CHANNELS 128
#define MAX_USERS_PER_CHANNEL 32

class CellTower {
private:
    int id;
    char location[32];
    Protocol* protocol;
    int totalBWkHz;
    int channelCount;

    StaticArray<UserDevice*, MAX_USERS_PER_CHANNEL> channels[MAX_CHANNELS];

public:
    CellTower(int towerId,
              const char* loc,
              Protocol* proto,
              int totalBW);

    void setupChannels();

    int getMaxUsersBySpectrum() const;

    bool addUser(UserDevice* user);

    const StaticArray<UserDevice*, MAX_USERS_PER_CHANNEL>&
    getChannel(int idx) const;

    int getChannelCount() const;
};
