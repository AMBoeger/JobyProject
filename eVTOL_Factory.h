#include <map>
#include <random>
#include <ctime>
#include "eVTOL.h"
#include "eVTOL_parameters.h"


//Factory that produces randomized eVTOL Lists for the simulator.
//Internal plane options are tracked via map[String, PlaneObjects]

class eVTOL_Factory_Config {
    public:
        static inline std::vector<eVTOL_parameters> parameters = {
            {"Alpha", 120, 320, 0.6, 1.6, 4, 0.25},
            {"Bravo", 100, 100, 0.2, 1.5, 5, 0.10},
            {"Charlie", 160, 220, 0.8, 2.2, 3, 0.05},
            {"Delta", 90, 120, 0.62, 0.8, 2, 0.22},
            {"Echo", 30, 150, 0.3, 5.8, 2, 0.61}
            };

};



class eVTOL_Factory {
    public:
        //Returns a list of possible planes
        static std::vector<std::string> getPlaneOptions() {
            std::vector<std::string> options;
            for(eVTOL_parameters p : eVTOL_Factory_Config::parameters) {
                options.push_back(p.type);
            }
            return options;
        }


        //Looks through it's internal list of plane options and generates a random n list of planes
        static std::vector<eVTOL> generate_PlaneList(int numberOfPlanes) {
            std::vector<eVTOL> RNG_list;
            std::minstd_rand generator(std::time(0)); 
            std::uniform_int_distribution<> dist(0, eVTOL_Factory_Config::parameters.size() - 1);

            //Randomly pick (uniformly) from plane options, to generate test set
            for(int i = 0; i < numberOfPlanes; i++) {
                RNG_list.push_back(eVTOL(eVTOL_Factory_Config::parameters[dist(generator)]));
            }
            return RNG_list;
        }

        //Adds a new plane option to the factory
        static void insert_planeType(const eVTOL_parameters& spec) {
                getPlaneOptions().push_back(spec.type);
        }

};



