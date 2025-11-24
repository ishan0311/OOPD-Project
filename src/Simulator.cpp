#include "../include/Simulator.h"

extern basicIO io;

Simulator::Simulator() : core(0), overheadPer100(0) {}

void Simulator::askCoreCapacity() {
    io.outputstring("Enter maximum total messages a single cellular core can handle (0 = no limit): ");
    int m = io.inputint();
    if (m < 0) m = 0;
    core.setMaxMessages(m);

    if (m > 0) {
        io.outputstring("Enter overhead messages per 100 real messages (0 for none): ");
        overheadPer100 = io.inputint();
        if (overheadPer100 < 0) overheadPer100 = 0;
    } else {
        overheadPer100 = 0;
    }
}

void Simulator::askUsers() {
    users.clear();

    int n = 0;

    // Keep asking until valid number of users is entered
    while (1) {
        io.outputstring("Enter number of user devices to simulate (max 1024): ");
        n = io.inputint();

        if (n > 0 && n <= MAX_USERS) {
            break; // valid
        }

        io.outputstring("Invalid number of users. Please enter a value between 1 and ");
        io.outputint(MAX_USERS);
        io.outputstring(".\n");
    }

    int mode = 0;
    // Keep asking until mode is 1 or 2
    while (1) {
        io.outputstring("\nChoose traffic assignment mode:\n");
        io.outputstring(" 1 - All users have SAME traffic type\n");
        io.outputstring(" 2 - Set traffic type for EACH user separately\n");
        io.outputstring("Enter choice: ");
        mode = io.inputint();

        if (mode == 1 || mode == 2) {
            break; // valid
        }

        io.outputstring("Invalid mode. Please enter 1 or 2.\n");
    }

    if (mode == 1) {
        // All users share same traffic type
        int t = 0;
        while (1) {
            io.outputstring("Select traffic type for ALL users (1=data, 2=voice, 3=both): ");
            t = io.inputint();
            if (t == 1 || t == 2 || t == 3) break;
            io.outputstring("Invalid traffic type. Please enter 1, 2, or 3.\n");
        }

        TrafficType tt;
        if (t == 1) tt = TRAFFIC_DATA;
        else if (t == 2) tt = TRAFFIC_VOICE;
        else tt = TRAFFIC_BOTH;

        int i;
        for (i = 0; i < n; ++i) {
            UserDevice u(i, tt);
            users.push_back(u);
        }
    } else { // mode == 2
        // Per-user traffic type
        int i;
        for (i = 0; i < n; ++i) {
            io.outputstring("User ");
            io.outputint(i);

            int t = 0;
            while (1) {
                io.outputstring(" traffic type (1=data, 2=voice, 3=both): ");
                t = io.inputint();
                if (t == 1 || t == 2 || t == 3) break;
                io.outputstring("Invalid traffic type. Please enter 1, 2, or 3.\n");
            }

            TrafficType tt;
            if (t == 1) tt = TRAFFIC_DATA;
            else if (t == 2) tt = TRAFFIC_VOICE;
            else tt = TRAFFIC_BOTH;

            UserDevice u(i, tt);
            users.push_back(u);
        }
    }
}

int Simulator::askGeneration() {
    io.outputstring("\nSelect generation to simulate:\n");
    io.outputstring(" 2 - 2G\n");
    io.outputstring(" 3 - 3G\n");
    io.outputstring(" 4 - 4G\n");
    io.outputstring(" 5 - 5G\n");
    io.outputstring("Enter choice: ");
    int g = io.inputint();
    return g;
}

void Simulator::run() {
    try {
        askCoreCapacity();
        askUsers();

        int gen = askGeneration();
        if (gen == 2) {
            simulateGeneration(p2g, 2);
        } else if (gen == 3) {
            simulateGeneration(p3g, 3);
        } else if (gen == 4) {
            simulateGeneration(p4g, 4);
        } else if (gen == 5) {
            simulateGeneration(p5g, 5);
        } else {
            throw SimulationException("Invalid generation choice.");
        }
    } catch (SimulationException& e) {
        io.errorstring("Simulation error: ");
        io.errorstring(e.what());
        io.terminate();
    }
}

template <typename P>
void Simulator::simulateGeneration(P& protocol, int gen) {
    int totalBandwidthKHz = 0;
    int antennas = 1;

    if (gen == 2 || gen == 3 || gen == 4) {
        io.outputstring("\nAssuming basic band (default 1 MHz).\n");
        io.outputstring("Enter total bandwidth in kHz (suggest 1000 for 1 MHz): ");
        totalBandwidthKHz = io.inputint();
        if (totalBandwidthKHz <= 0) totalBandwidthKHz = 1000;
    } else if (gen == 5) {
        io.outputstring("\n5G high band (default 10 MHz at 1800 MHz).\n");
        io.outputstring("Enter total bandwidth in kHz (suggest 10000 for 10 MHz): ");
        totalBandwidthKHz = io.inputint();
        if (totalBandwidthKHz <= 0) totalBandwidthKHz = 10000;
    }

    io.outputstring("Enter number of antennas to use (max ");
    io.outputint(protocol.maxAntennas());
    io.outputstring("): ");
    antennas = io.inputint();
    if (antennas <= 0) antennas = 1;
    if (antennas > protocol.maxAntennas()) antennas = protocol.maxAntennas();

    CellularCore localCore = core;
    CellTower tower(totalBandwidthKHz, antennas, &protocol, &localCore);

    FixedArray<int, MAX_USERS> acceptedUserIds;
    int realMessagesUsed = 0;   // only user messages, no overhead yet

    int acceptedUsers = tower.simulateUsers(users,
                                            acceptedUserIds,
                                            realMessagesUsed,
                                            overheadPer100);

    int maxSpectrum = tower.maxUsersBySpectrum();
    int perChannel = protocol.maxUsersPerChannel();
    int totalCh = tower.totalChannels();

    int channelsWithUsers = 0;
    if (perChannel > 0) {
        channelsWithUsers = acceptedUsers / perChannel;
        if (acceptedUsers % perChannel != 0) {
            channelsWithUsers++;
        }
    }

    // Now include overhead into total message count for reporting
    int totalMessagesUsed = realMessagesUsed;
    int overheadTotal = 0;

    if (core.getMaxMessages() > 0 && overheadPer100 > 0) {
        int overheadBlocks = realMessagesUsed / 100;
        overheadTotal = overheadBlocks * overheadPer100;
        totalMessagesUsed = realMessagesUsed + overheadTotal;
    }

    io.outputstring("\n===== Simulation Result for ");
    io.outputstring(protocol.name());
    io.outputstring(" =====\n");

    io.outputstring("Total bandwidth (kHz): ");
    io.outputint(totalBandwidthKHz);
    io.terminate();

    io.outputstring("Antennas used: ");
    io.outputint(antennas);
    io.terminate();

    io.outputstring("Max users allowed by spectrum: ");
    io.outputint(maxSpectrum);
    io.terminate();

    if (core.getMaxMessages() > 0) {
        io.outputstring("Core message limit (including overhead): ");
        io.outputint(core.getMaxMessages());
        io.terminate();

        io.outputstring("Real messages from users (no overhead): ");
        io.outputint(realMessagesUsed);
        io.terminate();

        io.outputstring("Overhead messages (per 100 real messages): ");
        io.outputint(overheadTotal);
        io.terminate();

        io.outputstring("Total messages used (real + overhead): ");
        io.outputint(totalMessagesUsed);
        io.terminate();
    } else {
        io.outputstring("Core message limit: no limit (0 specified)\n");
    }

    io.outputstring("Overhead per 100 real messages: ");
    io.outputint(overheadPer100);
    io.terminate();

    io.outputstring("Actual users accommodated by this tower: ");
    io.outputint(acceptedUsers);
    io.terminate();

    io.outputstring("Total channels available in this tower: ");
    io.outputint(totalCh);
    io.terminate();

    io.outputstring("Channels with at least one user: ");
    io.outputint(channelsWithUsers);
    io.terminate();

    // ----- FIRST CHANNEL (frequency band 0) -----
    io.outputstring("User IDs occupying the FIRST channel (channel index 0): ");
    if (acceptedUsers == 0 || perChannel <= 0) {
        io.outputstring("none");
        io.terminate();
    } else {
        int printed = 0;
        int i;
        for (i = 0; i < acceptedUserIds.size(); ++i) {
            int chIndex = i / perChannel;
            if (chIndex == 0) {
                if (printed > 0) {
                    io.outputstring(", ");
                }
                io.outputint(acceptedUserIds[i]);
                printed++;
            }
        }
        if (printed == 0) {
            io.outputstring("none");
        }
        io.terminate();
    }

    // ----- SPECIFIC CHANNEL INSPECTION (LOOP UNTIL -1) -----
    if (channelsWithUsers > 0 && perChannel > 0) {
        while (1) {
            io.outputstring("Enter channel index to inspect (-1 to stop, 0 to ");
            io.outputint(channelsWithUsers - 1);
            io.outputstring("): ");
            int ch = io.inputint();

            if (ch == -1) {
                // done inspecting channels
                break;
            }

            if (ch >= 0 && ch < channelsWithUsers) {
                io.outputstring("User IDs occupying channel ");
                io.outputint(ch);
                io.outputstring(": ");

                int printed = 0;
                int i;
                for (i = 0; i < acceptedUserIds.size(); ++i) {
                    int chIndex = i / perChannel;
                    if (chIndex == ch) {
                        if (printed > 0) {
                            io.outputstring(", ");
                        }
                        io.outputint(acceptedUserIds[i]);
                        printed++;
                    }
                }
                if (printed == 0) {
                    io.outputstring("none");
                }
                io.terminate();
            } else {
                io.outputstring("Invalid channel index. Please try again.\n");
            }
        }
    }

    // Extra requirement for 4G/5G: number of core instances to use full potential
    if (gen == 4 || gen == 5) {
        int coresNeeded = tower.coresNeededForFullPotential();
        io.outputstring("Cores needed to use full tower potential: ");
        io.outputint(coresNeeded);
        io.terminate();
    }
}

// Explicitly instantiate the template for the compiler
template void Simulator::simulateGeneration<Protocol2G>(Protocol2G&, int);
template void Simulator::simulateGeneration<Protocol3G>(Protocol3G&, int);
template void Simulator::simulateGeneration<Protocol4G>(Protocol4G&, int);
template void Simulator::simulateGeneration<Protocol5G>(Protocol5G&, int);
