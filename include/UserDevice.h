#pragma once

class Protocol;

class UserDevice {
private:
    int id;
    int channelIndex;
    Protocol* protocol;

public:
    UserDevice();
    UserDevice(int idValue);

    void setId(int idValue);
    int getId() const;

    void setChannelIndex(int idx);
    int getChannelIndex() const;

    void setProtocol(Protocol* p);
    Protocol* getProtocol() const;
};
