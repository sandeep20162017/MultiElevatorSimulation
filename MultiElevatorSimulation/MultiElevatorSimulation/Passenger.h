#pragma once
#include <string>
#include "Call.h"

class Passenger {

private:
    Call floorCall;
    Call carCall;
    std::string ID;

public:
   
    Passenger(Call floorCall, Call carCall, std::string ID) {
        this->floorCall = floorCall;
        this->carCall = carCall;
        this->ID = ID;
    }

    std::string getID() {
        return ID;
    }

    Call getFloorCall() {
        return floorCall;
    }

    Call getCarCall() {
        return carCall;
    }
};


