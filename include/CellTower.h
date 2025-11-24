#pragma once

#include "Protocols.h"
#include "CellularCore.h"
#include "UserDevice.h"
#include "FixedArray.h"

class CellTower {
private:
    int totalBandwidthKHz;
    int antennas;
    const Protocol* protocol;
    CellularCore* core;

public:
    CellTower(int bwKHz, int antennaCount, const Protocol* p, CellularCore* c);

    int getTotalBandwidthKHz() const;
    int getAntennas() const;

    int totalChannels() const;
    int maxUsersBySpectrum() const;

    // For 4G/5G: cores needed to use full tower potential
    int coresNeededForFullPotential() const;

    // Simulate assigning users, bounded by spectrum and core capacity
    // totalMessagesUsed = total REAL user messages (overhead not added here)
    // overheadPer100 = overhead messages generated per 100 real messages,
    //                  used to decide whether another user can be accepted.
    template <int MAX_USERS>
    int simulateUsers(FixedArray<UserDevice, MAX_USERS>& users,
                      FixedArray<int, MAX_USERS>& acceptedUserIds,
                      int& totalMessagesUsed,
                      int overheadPer100) const
    {
        totalMessagesUsed = 0;
        acceptedUserIds.clear();

        int acceptedUsers     = 0;
        int capacitySpectrum  = maxUsersBySpectrum();
        int maxMsgs           = core->getMaxMessages();
        int usersCount        = users.size();

        for (int i = 0; i < usersCount; ++i) {
            if (acceptedUsers >= capacitySpectrum) {
                // hit spectrum limit (per-frequency + total bandwidth)
                break;
            }

            UserDevice& u = users[i];
            int msgsForUser = protocol->messagesPerUser(u.getTrafficType());
            if (msgsForUser <= 0) msgsForUser = 1;

            // --- core limit including overhead per 100 messages ---
            if (maxMsgs > 0) {
                int newReal = totalMessagesUsed + msgsForUser;  // real msgs if we add this user

                if (overheadPer100 > 0) {
                    int newBlocks        = newReal / 100;
                    int newOverhead      = newBlocks * overheadPer100;
                    int totalWithOverhead = newReal + newOverhead;

                    if (totalWithOverhead > maxMsgs) {
                        // adding this user would exceed core capacity
                        break;
                    }
                } else {
                    // no overhead configured
                    if (newReal > maxMsgs) {
                        break;
                    }
                }
            }

            // Accept this user
            totalMessagesUsed += msgsForUser;
            ++acceptedUsers;
            acceptedUserIds.push_back(u.getId());
        }

        return acceptedUsers;
    }
};
