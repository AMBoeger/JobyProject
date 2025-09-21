#include "simulator.h"

const int SIMULATOR_HOURS = 5;
const int NUM_PLANES = 20;
const int NUM_CHARGERS = 3;
const bool DEBUG_FLAG = 1;

//ASSUMPTIONS
// 1. Within the same update, if a plane finishes charging, another plane can instantly take its spot.
// 2. Faults only occur during flight hours

int main() {
    Simulator sim;
    sim.setup(NUM_PLANES, NUM_CHARGERS);
    sim.run(SIMULATOR_HOURS, DEBUG_FLAG);
    sim.printStatistics();
    return 0;
}