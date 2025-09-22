#include "simulator.h"
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <unordered_map>
#include <iomanip>


void Simulator::run() {
    std::cout << "SIMULATION START\n\n";
    int totalCycles = config.SIMULATOR_HOURS / config.TIME_SLICE;
    for(int curCycle = 0; curCycle < totalCycles; curCycle++) {
        if(config.SIM_DEBUG_FLAG) { 
            debug_output();
        }
        
        checkFlying();
        checkChargers();
        updatePlaneStates();
    }
}

void Simulator::debug_output() {
    std::cout << "\nIncrement Simulator... TIME: " + std::to_string(this->TOTAL_TIME) << " hrs " << std::endl;
    this->TOTAL_TIME += config.TIME_SLICE;
    for(eVTOL plane : plane_list) {
        std::cout << plane.toString();
    }

}

void Simulator::checkFlying() {
        // Check Flying Planes
        for(int i = 0; i < plane_list.size(); i++) {
            eVTOL* currentPlane = &plane_list[i];
            if(currentPlane->isFlying()) {
                currentPlane->dischargeBattery_mi(config.TIME_SLICE * currentPlane->getCruiseSpeed_mph());
                currentPlane->testFault_discrete(config.TIME_SLICE);
            }
        }
}


//Charges planes parked at chargers, moves other planes into chargers if one is available, 
//Frees up charger if plane as at 100%
void Simulator::checkChargers() {
    for(int i = 0; i < charger_list.size(); i++) {
        eCharger& currCharger = charger_list[i];
        currCharger.chargePlane_ifOccupied(config.TIME_SLICE);
        currCharger.attachPlane_ifAvailable(charge_queue, config.TIME_SLICE);
    }
    return;
}

void Simulator::updateState_flying(eVTOL* currPlane) {
    if(currPlane->getBatteryLevel() <= config.MIN_BATTERY) {
        currPlane->setStatus(WAITING_TO_CHARGE);
        charge_queue.push(currPlane);
        return;
    }
    currPlane->updateFlightTime(config.TIME_SLICE);
    currPlane->updateDistance(config.TIME_SLICE * currPlane->getCruiseSpeed_mph());
}

void Simulator::updateState_charging(eVTOL* currPlane) {
    if(currPlane->getBatteryLevel() >= config.MAX_BATTERY) {
        currPlane->setStatus(FLYING);
        currPlane->updateTotalFlights();
    }
    currPlane->updateTimeCharged(config.TIME_SLICE);
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



void Simulator::setup() {

    //Create n number of chargers
    for(int i = 0; i < config.NUM_CHARGERS; i++) {
        this->charger_list.push_back(eCharger());
    }
    //Create NUM_PLANES list of planes
    this->plane_list = eVTOL_Factory::generate_PlaneList(config.NUM_PLANES);

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


//Crediting StackOverflow for pretty output template - Original console output was ugly.
void Simulator::printStatistics() {
    std::unordered_map<std::string, statistics> statsMap = gatherStatistics();
    const int typeWidth      = 12;
    const int flightWidth    = 12;
    const int distanceWidth  = 15;
    const int chargeWidth    = 15;
    const int totalWidth     = 10;
    const int faultsWidth    = 12;
    const int passMilesWidth = 20;

    std::cout << "\nSimulator Finished\n\nStatistics:\n";
    std::cout << std::left
              << std::setw(typeWidth)      << "Type"
              << std::setw(flightWidth)    << "Avg Flight"
              << std::setw(distanceWidth)  << "Avg Distance"
              << std::setw(chargeWidth)    << "Avg Charge"
              << std::setw(totalWidth)     << "Flights"
              << std::setw(faultsWidth)    << "Faults"
              << std::setw(passMilesWidth) << "Passenger Miles"
              << "\n";

    std::cout << std::string(typeWidth + flightWidth + distanceWidth + chargeWidth + totalWidth + faultsWidth + passMilesWidth, '-') << "\n";

    //Go through map of collected stats and print
    for (auto it = statsMap.begin(); it != statsMap.end(); ++it) {
        const std::string& type = it->first;
        const statistics& stats  = it->second;

        double avgFlight   = stats.flightTime / stats.totalFlights;
        double avgDistance = stats.distanceTraveled / stats.totalFlights;
        double avgCharge   = stats.timeCharged / stats.totalFlights;

        std::cout << std::left
                  << std::setw(typeWidth)      << type
                  << std::setw(flightWidth)    << std::fixed << std::setprecision(2) << avgFlight
                  << std::setw(distanceWidth)  << std::fixed << std::setprecision(2) << avgDistance
                  << std::setw(chargeWidth)    << std::fixed << std::setprecision(2) << avgCharge
                  << std::setw(totalWidth)     << stats.totalFlights
                  << std::setw(faultsWidth)    << stats.totalFaults
                  << std::setw(passMilesWidth) << std::fixed << std::setprecision(2) << stats.totalPassengerMiles
                  << "\n";
    }
    std::cout << "\n";
}