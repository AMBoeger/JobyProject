Work In Progress Joby Simulator


BUILD: ./build.sh
RUN: ./run.sh

Joby Plane Simulator Design Document

Overview
Sim_Confg: Generates object to configure a simulation. See main.

Simulator.setup() - This will generate a random list of n planes (and m chargers) based on templates provided by eVTOL factory and “attach” them to the simulator.

Simulator.run() - Simulate n planes operating in a basic state machine. If the plane is fully charged, it will immediately fly at full cruise speed. Once drained, the plane will attempt to charge, if all chargers are full, we wait for the next available charger. (See State Diagram)

Simulator.printStatistics() - Compiles data from all simulated planes and outputs various information to the console. HINT: Use after you have run the simulator.
