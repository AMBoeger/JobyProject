#include "simulator.h"




//ASSUMPTIONS
// 1. Within the same update, if a plane finishes charging, another plane can instantly take its spot.
// 2. Faults only occur during flight hours

int main() {
    Simulator sim;
    sim.setup(NUM_PLANES, NUM_CHARGERS);
    sim.run(SIMULATOR_HOURS);
    sim.printStatistics();
    return 0;
}