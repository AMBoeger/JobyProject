#include <string>
#pragma once

struct eVTOL_parameters {
    std::string type;
    double cruiseSpeed_mph;
    int batteryCapacity_kWh;
    double timeCharge_hrs;
    double energyConsumption_kWhmi;
    int passengerCount;
    double faultProb_hr;
};

struct statistics {
    double flightTime = 0;
    double distanceTraveled = 0;
    double timeCharged = 0;
    int totalFaults = 0;
    double totalPassengerMiles = 0;
    int    totalFlights = 1;
};

enum state {
    FLYING,
    WAITING_TO_CHARGE,
    CHARGING
};


