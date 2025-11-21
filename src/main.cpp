#include "../include/basicIO.h"
#include "../include/Protocol.h"
#include "../include/UserDevice.h"
#include "../include/CellTower.h"
#include "../include/CellularCore.h"
#include <thread>

// --------- Helpers to create users & print (no STL) ---------

void createUsers(UserDevice* arr, int count) {
    for (int i = 0; i < count; ++i) {
        arr[i].setId(i + 1);
    }
}

void printLine(const char* text) {
    io.outputstring(text);
    io.terminate();
}

void printLabelInt(const char* label, int value) {
    io.outputstring(label);
    io.outputint(value);
    io.terminate();
}

// --------- Simulation per generation (each can be threaded) ---------

void simulate2G() {
    try {
        Protocol2G p;
        CellularCore core(100000);                  // assumed core capacity
        CellTower tower(1, "2G-Tower", &p, 1000);   // 1 MHz = 1000 kHz

        int maxSpectrum = tower.getMaxUsersBySpectrum();
        int maxCore     = core.computeMaxUsersByCore(p);
        int maxUsers    = maxSpectrum;
        if (maxCore < maxUsers) maxUsers = maxCore;

        printLine("========== 2G Simulation ==========");
        printLabelInt("Max users by spectrum: ", maxSpectrum);
        printLabelInt("Approx max users by core: ", maxCore);
        printLabelInt("Effective max users (2G): ", maxUsers);

        static UserDevice users[512];
        if (maxUsers > 512) maxUsers = 512;
        createUsers(users, maxUsers);

        for (int i = 0; i < maxUsers; ++i) {
            tower.addUser(&users[i]);
        }

        const StaticArray<UserDevice*, MAX_USERS_PER_CHANNEL>& ch0 = tower.getChannel(0);
        printLine("Users in first 2G channel (first 200 kHz):");
        int sz = ch0.getSize();
        for (int i = 0; i < sz; ++i) {
            UserDevice* u = ch0.get(i);
            io.outputstring("  User ID = ");
            io.outputint(u->getId());
            io.terminate();
        }
    } catch (SimException& e) {
        io.errorstring("2G simulation error: ");
        io.errorstring(e.msg);
        io.terminate();
    }
}

void simulate3G() {
    try {
        Protocol3G p;
        CellularCore core(100000);
        CellTower tower(2, "3G-Tower", &p, 1000);   // 1 MHz

        int maxSpectrum = tower.getMaxUsersBySpectrum();
        int maxCore     = core.computeMaxUsersByCore(p);
        int maxUsers    = maxSpectrum;
        if (maxCore < maxUsers) maxUsers = maxCore;

        printLine("========== 3G Simulation ==========");
        printLabelInt("Max users by spectrum: ", maxSpectrum);
        printLabelInt("Approx max users by core: ", maxCore);
        printLabelInt("Effective max users (3G): ", maxUsers);

        static UserDevice users[512];
        if (maxUsers > 512) maxUsers = 512;
        createUsers(users, maxUsers);

        for (int i = 0; i < maxUsers; ++i) {
            tower.addUser(&users[i]);
        }

        const StaticArray<UserDevice*, MAX_USERS_PER_CHANNEL>& ch0 = tower.getChannel(0);
        printLine("Users in first 3G channel (first 200 kHz):");
        int sz = ch0.getSize();
        for (int i = 0; i < sz; ++i) {
            UserDevice* u = ch0.get(i);
            io.outputstring("  User ID = ");
            io.outputint(u->getId());
            io.terminate();
        }
    } catch (SimException& e) {
        io.errorstring("3G simulation error: ");
        io.errorstring(e.msg);
        io.terminate();
    }
}

void simulate4G() {
    try {
        Protocol4G p;
        CellularCore core(100000);
        CellTower tower(3, "4G-Tower", &p, 1000);   // 1 MHz total

        int maxSpectrumPerAntenna = tower.getMaxUsersBySpectrum();
        int antennas              = 4;
        int maxSpectrumTotal      = maxSpectrumPerAntenna * antennas;

        int maxPerCore  = core.computeMaxUsersByCore(p);
        int coresNeeded = core.computeNeededCores(p, maxSpectrumTotal);

        printLine("========== 4G Simulation ==========");
        printLabelInt("Max users by spectrum (per antenna): ", maxSpectrumPerAntenna);
        printLabelInt("Total 4G users with 4 antennas: ", maxSpectrumTotal);
        printLabelInt("Approx users per core (4G): ", maxPerCore);
        printLabelInt("Cores needed for full 4G potential: ", coresNeeded);

        int effectiveUsers = maxSpectrumTotal;
        if (maxPerCore * coresNeeded < effectiveUsers) {
            effectiveUsers = maxPerCore * coresNeeded;
        }

        static UserDevice users[1024];
        if (effectiveUsers > 1024) effectiveUsers = 1024;
        createUsers(users, effectiveUsers);

        for (int i = 0; i < effectiveUsers; ++i) {
            tower.addUser(&users[i]);
        }

        const StaticArray<UserDevice*, MAX_USERS_PER_CHANNEL>& ch0 = tower.getChannel(0);
        printLine("Users in first 4G channel (first 10 kHz):");
        int sz = ch0.getSize();
        for (int i = 0; i < sz; ++i) {
            UserDevice* u = ch0.get(i);
            io.outputstring("  User ID = ");
            io.outputint(u->getId());
            io.terminate();
        }
    } catch (SimException& e) {
        io.errorstring("4G simulation error: ");
        io.errorstring(e.msg);
        io.terminate();
    }
}

void simulate5G() {
    try {
        Protocol5G p;
        CellularCore core(100000);
        CellTower tower(4, "5G-Tower", &p, 10000); // 10 MHz block

        int maxSpectrumPerAntenna = tower.getMaxUsersBySpectrum();
        int antennas              = 16;
        int maxSpectrumTotal      = maxSpectrumPerAntenna * antennas;

        int maxPerCore  = core.computeMaxUsersByCore(p);
        int coresNeeded = core.computeNeededCores(p, maxSpectrumTotal);

        printLine("========== 5G Simulation ==========");
        printLabelInt("Max users by spectrum (per antenna): ", maxSpectrumPerAntenna);
        printLabelInt("Total 5G users with 16 antennas: ", maxSpectrumTotal);
        printLabelInt("Approx users per core (5G): ", maxPerCore);
        printLabelInt("Cores needed for full 5G potential: ", coresNeeded);

        int effectiveUsers = maxSpectrumTotal;
        if (maxPerCore * coresNeeded < effectiveUsers) {
            effectiveUsers = maxPerCore * coresNeeded;
        }

        static UserDevice users[5000];
        if (effectiveUsers > 5000) effectiveUsers = 5000;
        createUsers(users, effectiveUsers);

        for (int i = 0; i < effectiveUsers; ++i) {
            tower.addUser(&users[i]);
        }

        const StaticArray<UserDevice*, MAX_USERS_PER_CHANNEL>& ch0 = tower.getChannel(0);
        printLine("Users in first 5G channel (first 1 MHz):");
        int sz = ch0.getSize();
        for (int i = 0; i < sz; ++i) {
            UserDevice* u = ch0.get(i);
            io.outputstring("  User ID = ");
            io.outputint(u->getId());
            io.terminate();
        }
    } catch (SimException& e) {
        io.errorstring("5G simulation error: ");
        io.errorstring(e.msg);
        io.terminate();
    }
}

// --------- main: sequential threaded execution (A) ---------

int main() {
    io.outputstring("Starting Cellular Network Simulator...\n");

    std::thread t2(simulate2G);
    t2.join();

    std::thread t3(simulate3G);
    t3.join();

    std::thread t4(simulate4G);
    t4.join();

    std::thread t5(simulate5G);
    t5.join();

    io.outputstring("Simulation complete.\n");
    return 0;
}
