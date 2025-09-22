#include "eVTOL.h"
#include <iostream>
#include <iomanip>


void eVTOL::updateFlightTime(double time_hrs) {
    if(time_hrs < 0) {return;}
    this->flightStats.flightTime+= time_hrs;
}
void eVTOL::updateDistance(double miles) {
    if(miles < 0) {return;}
    this->flightStats.distanceTraveled+= miles;
}
void eVTOL::updateTimeCharged(double time_hrs) {
    if(time_hrs < 0) {return;}
    this->flightStats.timeCharged+= time_hrs;
}
void eVTOL::updateTotalFlights() {
    this->flightStats.totalFlights++;
}

double eVTOL::chargeBattery_hrs(double hours) {
    if(hours < 0) {return 0;}
    return adjustBattery_kWh(hours * this->parameters.batteryCapacity_kWh * (1 / this->parameters.timeCharge_hrs));
}

double eVTOL::dischargeBattery_mi(double distance_mi) {
    if(distance_mi < 0) {return 0;}
    return adjustBattery_kWh(-distance_mi * this->parameters.energyConsumption_kWhmi);
}

double eVTOL::adjustBattery_kWh(double charge_kWh){
    double amountChanged_kWh;
    //Check for overcharge
    if(this->currBatteryLevel_kwH + charge_kWh >= parameters.batteryCapacity_kWh) {
        amountChanged_kWh = parameters.batteryCapacity_kWh - this->currBatteryLevel_kwH;
        this->currBatteryLevel_kwH = parameters.batteryCapacity_kWh;
        return amountChanged_kWh;
    }

    //Check for complete drain
    if(this->currBatteryLevel_kwH + charge_kWh <= 0) {
        amountChanged_kWh = this->currBatteryLevel_kwH;
        this->currBatteryLevel_kwH = 0;
        return amountChanged_kWh;
    }

    //We are inside charge range
    amountChanged_kWh = charge_kWh;
    this->currBatteryLevel_kwH += charge_kWh;
    return amountChanged_kWh;
}

double eVTOL::getBatteryLevel() {
    return this->currBatteryLevel_kwH / this->parameters.batteryCapacity_kWh;
}

//Tests to see if fault generated
void eVTOL::testFault_discrete(double hrs) {
    std::random_device random; 
    std::minstd_rand generator(random());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    //We need to reduce the hourly fault rate to a single time slice fault rate  --> pr = 1 - (1 - faults per hour) ^ hours
    double probability = 1.0 - std::pow(1.0 - this->parameters.faultProb_hr, hrs);

    //Update faults if we generated one
    this->flightStats.totalFaults += (dist(generator) < probability);
}

std::string eVTOL::string_Status() {
    switch(this->getStatus()) {
        case FLYING:
            return "FLYING";
        case WAITING_TO_CHARGE:
            return "WAITING_TO_CHARGE";
        case CHARGING:
            return "CHARGING";
        default:
            return "BAD STATUS";
    }
    return "BAD STATUS";
}

std::string eVTOL::toString() {
    std::ostringstream ss;
    ss << std::left << std::setw(12) << this->getType();
    ss << std::setw(8)  << ("ID:" + std::to_string(this->uid));
    ss << std::setw(12) << (std::to_string(this->currBatteryLevel_kwH) + " kWh ");
    ss << std::setw(28) << this->string_Status();
    ss << "\n";
    return ss.str();
}

uint16_t eVTOL::uid_next = 0;

