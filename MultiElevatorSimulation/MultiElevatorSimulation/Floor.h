#pragma once
#include <deque>
#include "Passenger.h"

class Floor {
    private:  
        std::deque<Passenger *> passengers;        
        int ID;
        bool DEBUG = false;

    public:   
        Floor(int ID) {
        
            this->passengers = this->getPassengers();
            this->ID = ID;
        }
        void generatePassenger(int numberOfFloors)  {
            //TODO : for testing
        }
   
        std::deque<Passenger*> getPassengers() {
            return passengers;
        }

        int getID() {
            return ID;
        }
};