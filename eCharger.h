class eCharger {
    private:
        eVTOL* currentPlane = NULL;

    public:
        bool isOccupied() {return currentPlane != NULL;}
        void makeAvailable() {currentPlane = NULL;}
        void occupyCharger(eVTOL* plane) {currentPlane = plane;}
        eVTOL* getPlane() {return currentPlane;}

};