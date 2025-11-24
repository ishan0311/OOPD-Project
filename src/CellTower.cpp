#include "../include/CellTower.h"

CellTower::CellTower(int bwKHz, int antennaCount, const Protocol* p, CellularCore* c)
    : totalBandwidthKHz(bwKHz), antennas(antennaCount), protocol(p), core(c) {}

int CellTower::getTotalBandwidthKHz() const {
    return totalBandwidthKHz;
}

int CellTower::getAntennas() const {
    return antennas;
}

int CellTower::totalChannels() const {
    int perAntenna = totalBandwidthKHz / protocol->channelBandwidthKHz();
    if (perAntenna <= 0) return 0;
    return perAntenna * antennas;
}

int CellTower::maxUsersBySpectrum() const {
    return totalChannels() * protocol->maxUsersPerChannel();
}

int CellTower::coresNeededForFullPotential() const {
    if (!core) return 0;
    int maxMsgsPerCore = core->getMaxMessages();
    if (maxMsgsPerCore <= 0) return 0;

    int worstMessagesPerUser = protocol->messagesPerUser(TRAFFIC_BOTH);
    if (worstMessagesPerUser <= 0) worstMessagesPerUser = 10;

    int fullUsers = maxUsersBySpectrum();
    int totalMessagesNeeded = fullUsers * worstMessagesPerUser;

    int cores = totalMessagesNeeded / maxMsgsPerCore;
    if (totalMessagesNeeded % maxMsgsPerCore != 0) {
        cores++;
    }
    if (cores < 1) cores = 1;

    return cores;
}
