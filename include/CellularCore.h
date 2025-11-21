#pragma once

#include "Protocol.h"
#include "SimException.h"

class CellularCore {
private:
    int maxMessagesPerCore;

public:
    CellularCore(int maxMessages);

    int computeMaxUsersByCore(const Protocol& proto) const;
    int computeNeededCores(const Protocol& proto, int totalUsers) const;
};
