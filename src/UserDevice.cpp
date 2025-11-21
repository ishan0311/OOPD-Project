#include "../include/UserDevice.h"
#include "../include/Protocol.h"

UserDevice::UserDevice()
    : id(0), channelIndex(-1), protocol(0) {}

UserDevice::UserDevice(int idValue)
    : id(idValue), channelIndex(-1), protocol(0) {}

void UserDevice::setId(int idValue) {
    id = idValue;
}

int UserDevice::getId() const {
    return id;
}

void UserDevice::setChannelIndex(int idx) {
    channelIndex = idx;
}

int UserDevice::getChannelIndex() const {
    return channelIndex;
}

void UserDevice::setProtocol(Protocol* p) {
    protocol = p;
}

Protocol* UserDevice::getProtocol() const {
    return protocol;
}
