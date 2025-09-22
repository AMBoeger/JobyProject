#include "eVTOL.h"
#include "eCharger.h"
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include"definitions.h"
#include "eVTOL_Factory.h"

//Future Work - Remove definitions.h and create a sim config object
// class Sim_Config {
//     public:
//         const uint16_t NUM_PLANES;
//         const uint16_t NUM_CHARGERS;
//         const bool SIM_DEBUG_FLAG;
//         const uint16_t SIMULATOR_HOURS;
//         const double TIME_SLICE;

//         Sim_Config(uint16_t v_count, uint16_t c_count, bool debug,
//                     uint16_t hours , double timeSlice)
//                 : 
//                 NUM_PLANES(v_count),
//                 NUM_CHARGERS(c_count),
//                 SIM_DEBUG_FLAG(debug),
//                 SIMULATOR_HOURS(hours),
//                 TIME_SLICE(timeSlice)
//             {}
// };

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
