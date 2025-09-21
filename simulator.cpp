#include "simulator.h"
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <unordered_map>

const eVTOL_template* Simulator::planeOptions[] = {&Alpha, &Bravo, &Charlie, &Delta, &Echo};

void Simulator::run(double hours, bool DEBUG_FLAG) {
    std::cout << "SIMULATION START\n\n";
    int totalCycles = hours / TIME_SLICE;
    for(int curCycle = 0; curCycle < totalCycles; curCycle++) {
        if(DEBUG_FLAG) { 
            debug_output();
        }
        
        checkFlying();
        checkChargers();
        updatePlaneStates();
    }
}

void Simulator::debug_output() {
    std::cout << "\nIncrement Simulator... TIME: " + std::to_string(this->TOTAL_TIME) << " hrs " << std::endl;
    this->TOTAL_TIME += TIME_SLICE;
    for(eVTOL plane : plane_list) {
        std::cout << plane.toString();
    }

}
void Simulator::checkFlying() {
        // Check Flying Planes
        for(int i = 0; i < plane_list.size(); i++) {
            eVTOL* currentPlane = &plane_list[i];
            if(currentPlane->isFlying()) {
                currentPlane->dischargeBattery_mi(TIME_SLICE * currentPlane->getCruiseSpeed_mph());
                currentPlane->testFault_discrete(TIME_SLICE);
            }
        }
}


//Charges planes parked at chargers, moves planes into chargers if one is available, 
//Frees up charger if plane as at 100%
void Simulator::checkChargers() {
    for(int i = 0; i < charger_list.size(); i++) {
        eCharger* currCharger = &charger_list[i];

        //Open Charger, Available Plane
        if(!currCharger->isOccupied() && charge_queue.size()) {
            eVTOL* currPlane = charge_queue.front();
            charge_queue.pop();
            currCharger->occupyCharger(currPlane);
            currPlane->setStatus(CHARGING);
            currPlane->chargeBattery_hrs(TIME_SLICE);

        //Charge other Planes currently at chargers
        } else if(currCharger->isOccupied()) {
            eVTOL* currPlane = currCharger->getPlane();
            currPlane->chargeBattery_hrs(TIME_SLICE);
            if(currPlane->getBatteryLevel() >= 0.999) {
                currCharger->makeAvailable();
            }
        }
    }
    return;
}

//State Machine Logic, also updates stats based on state
void Simulator::updatePlaneStates() {
    for(int i = 0; i < plane_list.size(); i++) {
        eVTOL* currPlane = &plane_list[i];
        switch(currPlane->getStatus()) {
            case FLYING:
                if(currPlane->getBatteryLevel() <= 0.001) {
                    currPlane->setStatus(WAITING_TO_CHARGE);
                    charge_queue.push(currPlane);
                    continue;
                }
                currPlane->updateFlightTime(TIME_SLICE);
                currPlane->updateDistance(TIME_SLICE * currPlane->getCruiseSpeed_mph());
                break;

            case WAITING_TO_CHARGE:
                break;

            case CHARGING:
                if(currPlane->getBatteryLevel() >= 0.999) {
                    currPlane->setStatus(FLYING);
                }
                currPlane->updateTimeCharged(TIME_SLICE);
                break;

            default:
                //Need Error Handling
                break;
        }

    }
}

void Simulator::setup (int vehicle_count, int charger_count) {
    std::vector<eVTOL> myPlanes;
    std::vector<eCharger> myChargers;

    //Setup RNG Portion
    std::minstd_rand generator(std::time(0)); 
    std::uniform_int_distribution<> dist(0, sizeof(planeOptions) / sizeof(planeOptions[0]) - 1);

    //Create n number of chargers
    for(int i = 0; i < charger_count; i++) {
        myChargers.push_back(eCharger());
    }

    //Randomly pick (uniformly) from plane options, to generate test set
    for(int i = 0; i < vehicle_count; i++) {
        myPlanes.push_back(eVTOL(planeOptions[dist(generator)]));
    }

    //Attach to simulator
    this->plane_list = myPlanes;
    this->charger_list = myChargers;
    return;
}
