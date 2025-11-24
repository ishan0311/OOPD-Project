#include "../include/CellularCore.h"

CellularCore::CellularCore() : maxMessages(0) {}

CellularCore::CellularCore(int maxMsgs) : maxMessages(maxMsgs) {}

void CellularCore::setMaxMessages(int m) {
    maxMessages = m;
}

int CellularCore::getMaxMessages() const {
    return maxMessages;
}
