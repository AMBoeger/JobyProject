#include "eVTOL.h"
#include "eCharger.h"
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include"definitions.h"
#include "eVTOL_Factory.h"



class Simulator {
    public:
        void setup(int vehicle_count, int charger_count);
        void run(double hours);
        void printStatistics();


    private:
        double TOTAL_TIME = 0;
        std::vector<eCharger> charger_list;
        std::vector<eVTOL> plane_list;
        std::queue<eVTOL*> charge_queue;

        void checkChargers();
        void checkFlying();
        void updatePlaneStates();
        void debug_output();
        void updateState_flying(eVTOL* plane);
        void updateState_charging(eVTOL* plane);
        std::unordered_map<std::string, statistics> gatherStatistics();

};
