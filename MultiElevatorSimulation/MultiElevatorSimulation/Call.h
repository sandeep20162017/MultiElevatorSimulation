#pragma once
#include <string>
class Call {

private:
    int type; // 1 - floor call, 0 - car call
    int passage; // 1- P1, 2 - P2, 3 - P3
    int floor; // floor call - from where, car call - to where
    int direction; // 1 - Up, 0 - Down
    std::string ID;
    bool specialCall;

public:
    Call() {}
    Call(int type, int floor, int direction, std::string ID) {
        this->type = type;
        this->floor = floor;
        this->direction = direction;
        this->ID = ID;
        this->specialCall = false;
    }

    void setPassage(int passage) {
        this->passage = passage;
    }

    int getType() {
        return type;
    }

public:
    int getFloor() {
        return floor;
    }

    int getPassage() {
        return passage;
    }

    int getDirection() {
        return direction;
    }

    std::string getID() {
        return ID;
    }

    void setSpecialCall(bool specialCall) {
        this->specialCall = specialCall;
    }

    bool isSpecialCall() {
        return specialCall;
    }
};