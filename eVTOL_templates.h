#include <string>
#pragma once

typedef struct {
    const std::string type;
    const double cruiseSpeed_mph;
    const int batteryCapacity_kWh;
    const double timeCharge_hrs;
    const double energyConsumption_kWhmi;
    const int passengerCount;
    const double faultProb_hr;
} eVTOL_template;

#define numberOfTemplates 5
extern eVTOL_template Alpha;
extern eVTOL_template Bravo;
extern eVTOL_template Charlie;
extern eVTOL_template Delta;
extern eVTOL_template Echo;