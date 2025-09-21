#include "eVTOL.h"
#include "eCharger.h"
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>


class Simulator {
    public:
        void setup(int vehicle_count, int charger_count);
        void run(double hours, bool DEBUG_FLAG);
        void printStatistics();


    private:
        static const eVTOL_template* planeOptions[numberOfTemplates];
        const double TIME_SLICE = 0.01;   //Frequency is 1/100 hour
        double TOTAL_TIME = 0;
        std::vector<eCharger> charger_list;
        std::vector<eVTOL> plane_list;
        std::queue<eVTOL*> charge_queue;

        void checkChargers();
        void checkFlying();
        void updatePlaneStates();
        void debug_output();
        std::unordered_map<std::string, statistics> gatherStatistics();

};
