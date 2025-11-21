#pragma once

#include "SimException.h"
#include "Utils.h"

// Base protocol class
class Protocol {
protected:
    char name[16];
    int usersPerChannel;
    int channelBWkHz;
    int messagesPerUser;
    int overheadPer100;

public:
    Protocol(const char* nm,
             int upc,
             int cbw,
             int msgPerUser,
             int overhead);

    const char* getName() const;
    int getUsersPerChannel() const;
    int getChannelBWkHz() const;
    int getMessagesPerUser() const;
    int getOverheadPer100Messages() const;

    // NEW: allow changing overhead at runtime
    void setOverheadPer100Messages(int ov);

    int computeMaxUsersBySpectrum(int totalBWkHz) const;
};

// 2G
class Protocol2G : public Protocol {
public:
    Protocol2G();
};

// 3G
class Protocol3G : public Protocol {
public:
    Protocol3G();
};

// 4G
class Protocol4G : public Protocol {
public:
    Protocol4G();
};

// 5G
class Protocol5G : public Protocol {
public:
    Protocol5G();
};
