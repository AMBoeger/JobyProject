#include "eVTOL_parameters.h"
#include <string>
#include <random>
#include <ctime>
#pragma once


class eVTOL{
     friend class eVTOL_Factory;
    private:
        statistics flightStats;
        double currBatteryLevel_kwH;
        double adjustBattery_kWh(double kWh);
        state status;
        eVTOL();
        eVTOL(eVTOL_parameters& params) : parameters(params) {
            //Planes start fully charged and taking off.
            currBatteryLevel_kwH = params.batteryCapacity_kWh;
            status = FLYING;
            uid = uid_next++;
        }
        static uint16_t uid_next;

    public:
        eVTOL_parameters& parameters;
        uint16_t uid;

        // Updating information
        void updateFlightTime(double time_hrs);
        void updateDistance(double miles);
        void updateTimeCharged(double time_hrs);
        void updateTotalFlights();
        void testFault_discrete(double hrs);


        // Battery interfacing functions
        double chargeBattery_hrs(double charge_kWh);
        double dischargeBattery_mi(double distance_mi);

        //Returns battery level 0-100%
        double getBatteryLevel();

        //Returns battery level in kWh
        double getkWhs() { return currBatteryLevel_kwH;}

        //Flying and Landing functions
        bool isFlying() { return status == FLYING;}
        void setStatus(state newState) {status = newState;}
        state getStatus() {return status;}

        //Get parameters
        double getCruiseSpeed_mph() {return parameters.cruiseSpeed_mph;}
        int    getPassengerCount() {return parameters.passengerCount;}
        std::string getType() { return parameters.type;}

        //Get Plane Stats
        std::string toString();
        std::string string_Status();
        double getFlightTime()  {return flightStats.flightTime;}
        double getDistanceTraveled() {return flightStats.distanceTraveled;}
        double getTimeCharged() {return flightStats.timeCharged;}
        double getTotalFaults() {return flightStats.totalFaults;}
        double getTotalPassengerMiles() {return flightStats.totalPassengerMiles;}
        int    getTotalFlights() {return flightStats.totalFlights;}

};