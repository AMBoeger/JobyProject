#include "eVTOL_templates.h"
#include <string>
#include <random>
#include <ctime>


struct statistics {
    double flightTime = 0;
    double distanceTraveled = 0;
    double timeCharged = 0;
    double totalFaults = 0;
    double totalPassengerMiles = 0;
    int    totalFlights = 1;
};

enum state {
    FLYING,
    WAITING_TO_CHARGE,
    CHARGING
};

class eVTOL{
    private:
        statistics flightStats;
        double currBatteryLevel_kwH;
        double adjustBattery_kWh(double kWh);
        state status;


    public:
        eVTOL(const eVTOL_template *params) : parameters(params) {
            //Planes start fully charged and taking off.
            currBatteryLevel_kwH = params->batteryCapacity_kWh;
            status = FLYING;
        }
        const eVTOL_template *parameters;

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
        double getCruiseSpeed_mph() {return parameters->cruiseSpeed_mph;}
        int    getPassengerCount() {return parameters->passengerCount;}
        std::string getType() { return parameters->type;}

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