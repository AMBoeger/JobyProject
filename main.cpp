#include "simulator.h"




//ASSUMPTIONS
// 1. Within the same update, if a plane finishes charging, another plane can instantly take its spot.
// 2. Faults only occur during flight hours
// 3. Planes still wait a single time slice to transition from FLY -> CHARGE if there is an open charger .... (Waiting to Charge State)

int main() {

    //Example Default Config
    uint16_t numPlanes     = 20;
    uint16_t numChargers   = 3;
    bool debugFlag         = true;
    uint16_t simHours      = 3;
    double timeSlice       = 0.01;  // hours per time slice
    double maxBattery      = 1.00; // kWh
    double minBattery      = 0.00;  // kWh
    Sim_Config basic_config(numPlanes, numChargers, debugFlag, simHours, timeSlice, maxBattery, minBattery);




    Simulator sim(basic_config);
    sim.setup();
    sim.run();
    sim.printStatistics();
    return 0;
}