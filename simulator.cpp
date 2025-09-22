#include "simulator.h"
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <unordered_map>

void Simulator::run(double hours) {
    std::cout << "SIMULATION START\n\n";
    int totalCycles = hours / TIME_SLICE;
    for(int curCycle = 0; curCycle < totalCycles; curCycle++) {
        if(SIM_DEBBUGFLAG) { 
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


//Charges planes parked at chargers, moves other planes into chargers if one is available, 
//Frees up charger if plane as at 100%
void Simulator::checkChargers() {
    for(int i = 0; i < charger_list.size(); i++) {
        eCharger& currCharger = charger_list[i];
        currCharger.chargePlane_ifOccupied();
        currCharger.attachPlane_ifAvailable(charge_queue);
    }
    return;
}

void Simulator::updateState_flying(eVTOL* currPlane) {
    if(currPlane->getBatteryLevel() <= MIN_BATTERY_LVL) {
        currPlane->setStatus(WAITING_TO_CHARGE);
        charge_queue.push(currPlane);
        return;
    }
    currPlane->updateFlightTime(TIME_SLICE);
    currPlane->updateDistance(TIME_SLICE * currPlane->getCruiseSpeed_mph());
}

void Simulator::updateState_charging(eVTOL* currPlane) {
    if(currPlane->getBatteryLevel() >= MAX_BATTERY_LVL) {
        currPlane->setStatus(FLYING);
        currPlane->updateTotalFlights();
    }
    currPlane->updateTimeCharged(TIME_SLICE);
}

//Main State Machine Logic
void Simulator::updatePlaneStates() {
    for(int i = 0; i < plane_list.size(); i++) {
        eVTOL* currPlane = &plane_list[i];
        switch(currPlane->getStatus()) {
            case FLYING:
                updateState_flying(currPlane);
                break;

            case WAITING_TO_CHARGE:
                //Just keep waiting
                break;

            case CHARGING:
                updateState_charging(currPlane);
                break;

            default:
                //Need Error Handling
                break;
        }
    }
}



void Simulator::setup (int vehicle_count, int charger_count) {

    //Create n number of chargers
    for(int i = 0; i < charger_count; i++) {
        this->charger_list.push_back(eCharger());
    }
    //Create NUM_PLANES list of planes
    this->plane_list = eVTOL_Factory::generate_PlaneList(NUM_PLANES);

    return;
}

//Search through map, if found combine stats, else insert empty stats table per present plane type
std::unordered_map<std::string, statistics> Simulator::gatherStatistics() {
    std::unordered_map<std::string, statistics> statsMap;
    for(int i = 0; i < this->plane_list.size(); i++) {
        statistics* curStats;
        eVTOL curPlane = this->plane_list[i];
        if((statsMap.find(curPlane.getType()) == statsMap.end())) {
            statistics newStats;
            statsMap[curPlane.getType()] = newStats;
            curStats = &statsMap[curPlane.getType()];
        } else {
            curStats = &statsMap[curPlane.getType()];
        }
        curStats->distanceTraveled += curPlane.getDistanceTraveled();
        curStats->flightTime += curPlane.getFlightTime();
        curStats->totalFlights += curPlane.getTotalFlights();
        curStats->timeCharged += curPlane.getTimeCharged();
        curStats->totalFaults += curPlane.getTotalFaults();
        curStats->totalPassengerMiles += curPlane.getPassengerCount() * curPlane.getDistanceTraveled();
    }
    return statsMap;
}


void Simulator::printStatistics() {
    std::unordered_map<std::string, statistics> statsMap = gatherStatistics();
    //Print Out Stats
    std::cout << "\nSimulator Finished \n\nStatistics:\n";
    auto it = statsMap.begin();
    for(auto it = statsMap.begin(); it != statsMap.end(); it++) {
        std::cout << "Type: " << it->first << std::endl;
        std::cout << "Average Flight Time: " << it->second.flightTime / it->second.totalFlights << " hrs\n";
        std::cout << "Average Distance Traveled: " << it->second.distanceTraveled / it->second.totalFlights << " mi\n";
        std::cout << "Average Charging Time: " << it->second.timeCharged / it->second.totalFlights << " hrs\n";
        std::cout << "Total Flights: " << it->second.totalFlights << std::endl;
        std::cout << "Total Faults: " << it->second.totalFaults << std::endl;
        std::cout << "Total Passenger Miles: " << it->second.totalPassengerMiles << " mi\n";
        std::cout << "\n\n";
    }
}