#include "eCharger.h"


void eCharger::chargePlane_ifOccupied() {
        if(this->isOccupied()) {
            eVTOL* currPlane = this->getPlane();
            currPlane->chargeBattery_hrs(TIME_SLICE);
            if(currPlane->getBatteryLevel() >= 1.0) {
                this->makeAvailable();
            }
        }
}

void eCharger::attachPlane_ifAvailable(std::queue<eVTOL*> &charge_queue) {
       //Open Charger, Available Plane
        if(!this->isOccupied() && charge_queue.size()) {
            eVTOL* plane = charge_queue.front();
            charge_queue.pop();
            this->occupyCharger(plane);
            plane->setStatus(CHARGING);
            plane->chargeBattery_hrs(TIME_SLICE);
        }
}