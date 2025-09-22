#include <queue>
#include "eVTOL.h"
#include "definitions.h"
#pragma once


class eCharger {
    private:
        eVTOL* currentPlane = NULL;

    public:
        bool isOccupied() {return currentPlane != NULL;}
        void makeAvailable() {currentPlane = NULL;}
        void occupyCharger(eVTOL* plane) {currentPlane = plane;}
        eVTOL* getPlane() {return currentPlane;}
        void chargePlane_ifOccupied(const double TIME_SLICE);
        void attachPlane_ifAvailable(std::queue<eVTOL*> &charge_queue, const double TIME_SLICE);
};

