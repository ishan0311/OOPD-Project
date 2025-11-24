#pragma once

class CellularCore {
private:
    int maxMessages; // allowed messages per core
public:
    CellularCore();
    CellularCore(int maxMsgs);

    void setMaxMessages(int m);
    int getMaxMessages() const;
};
