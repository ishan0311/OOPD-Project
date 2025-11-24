#include "../include/basicIO.h"
#include "../include/Simulator.h"
#include "../include/Thread.h"

extern basicIO io;

void simulationThreadEntry(void* arg) {
    Simulator* sim = (Simulator*)arg;
    sim->run();
}

int main() {
    io.activateInput();

    while (1) {
        io.outputstring("==== Cellular Network Simulator ====\n");
        io.outputstring("1. Run simulation\n");
        io.outputstring("2. Exit\n");
        io.outputstring("Enter choice: ");

        int choice = io.inputint();

        if (choice == 2) {
            io.outputstring("Exiting simulator.\n");
            return 0;   // immediate exit from start
        } else if (choice == 1) {
            Simulator simulator;
            SimpleThread t(simulationThreadEntry, (void*)&simulator);
            t.start();
            // after one simulation finishes, loop back to menu
        } else {
            io.outputstring("Invalid choice. Please try again.\n");
        }
    }

    return 0; // never reached, but keeps compiler happy
}
