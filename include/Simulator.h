#pragma once

#include "Protocols.h"
#include "CellularCore.h"
#include "CellTower.h"
#include "UserDevice.h"
#include "FixedArray.h"
#include "SimulationException.h"
#include "basicIO.h"

class Simulator {
private:
    Protocol2G p2g;
    Protocol3G p3g;
    Protocol4G p4g;
    Protocol5G p5g;
    CellularCore core;

    static const int MAX_USERS = 1024;
    FixedArray<UserDevice, MAX_USERS> users;
    int overheadPer100;
    void askCoreCapacity();
    void askUsers();
    int askGeneration();

    template <typename P>
    void simulateGeneration(P& protocol, int gen);

public:
    Simulator();
    void run();
};
