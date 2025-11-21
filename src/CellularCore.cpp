#include "../include/CellularCore.h"

CellularCore::CellularCore(int maxMessages)
    : maxMessagesPerCore(maxMessages) {
    if (maxMessagesPerCore <= 0) {
        throw SimException("Core capacity must be positive");
    }
}

int CellularCore::computeMaxUsersByCore(const Protocol& proto) const {
    int msg = proto.getMessagesPerUser();
    int ov  = proto.getOverheadPer100Messages();
    int loadPerUser = msg + (msg * ov) / 100;
    if (loadPerUser <= 0) {
        throw SimException("Invalid load per user");
    }
    return maxMessagesPerCore / loadPerUser;
}

int CellularCore::computeNeededCores(const Protocol& proto, int totalUsers) const {
    int perCore = computeMaxUsersByCore(proto);
    if (perCore <= 0) {
        throw SimException("Per-core capacity zero");
    }
    int cores = totalUsers / perCore;
    if (totalUsers % perCore != 0) cores++;
    return cores;
}
