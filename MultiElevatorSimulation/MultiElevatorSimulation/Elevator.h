#pragma once
#include <iostream>
#include <thread>
#include <iterator>
#include <list>
#include <deque>
#include "Call.h"
#include "Comparator.h"
#include "Floor.h"
using namespace std;

    class Elevator {

    private :
 
     list<Floor> floorCalls; // Holds floorCalls
     list<Call> carCalls; // Holds carCalls
     Call *elevatorCall; 
     std::deque<Call> *sequence;

    private :        
     int algorithm; // Set at the time of elevator creation
     int ID;
     int currentFloor;
     int direction; // 1- Up, 0 - Down
     bool idle = true;
     bool DEBUG = false;

     int passengerLoadingTime; // static
     int passengerUnloadingTime; // static
     int velocity; // static
     int capacity; // 8, can be deduced from population
     int interFloorHeight; // static
     int N;
     int L;

    public :
       Elevator() {};
        Elevator(int ID, int algorithm, int passengerLoadingTime, int passengerUnloadingTime,
        int velocity, int capacity, int interFloorHeight) {
        this->ID = ID;
        this->algorithm = algorithm;
        this->passengerLoadingTime = passengerLoadingTime;
        this->passengerUnloadingTime = passengerUnloadingTime;
        this->velocity = velocity;
        this->capacity = capacity;
        this->interFloorHeight = interFloorHeight;
        
        //create callqueue
        deque<Call> *comparatorQueue;
        comparatorQueue->push_back(*elevatorCall);

        //put in sequence
        this->sequence = comparatorQueue;
    }

    public :
      void setN(int n) { this->N = n; }

      void setL(int l) { L = l; }

     void setCurrentFloor(int currentFloor) { this->currentFloor = currentFloor; }

    /**
     * Used during Elevator initialization
     */
     void setDirection(int direction) { this->direction = direction; }

     int getCapacity() { return capacity; }

     int getPassengerLoadingTime() {
        return passengerLoadingTime;
    }

     int getPassengerUnloadingTime() {
        return passengerUnloadingTime;
    }

     int getVelocity() {
        return velocity;
    }

     int getInterFloorHeight() {
        return interFloorHeight;
    }

     deque<Call> *getSequence() { return nullptr; } //TODO

     bool isIdle() { return idle; }

     int getCurrentFloor() { return currentFloor; }

     int getID() {
        return ID;
    }

     int getN() {
        return N;
    }

     int getL() {
        return L;
    }

    /**
     * Generates a carCall with floor 0 in order to relocate the car to the lobby.
     * The idea is to reduce the waiting time for future passengers arriving at the lobby.
     */
     void upPeakThread() {
         std::thread carThread(&Elevator::isIdle);
         while (true) {
             // Check if the elevator is idl

             if (isIdle()) {

                 std::this_thread::sleep_for(2000ms);

                 // Check if the elevator is still idle and
                  // is not already on the main floor
                 if (isIdle() && getCurrentFloor() != 0) {

                     // Create the carCall and add it to the sequence
                     Call* tempCall = new Call(0, 0, 0, "");
                     tempCall->setPassage(1);
                     tempCall->setSpecialCall(true);

                     //update sequence
                     getSequence()->push_back(*tempCall);
                 }

             }
             //wait for carThread to finish
             carThread.join();
         }
         
     }

     void performJobThread() {
         std::thread jobThread(&Elevator::performJob); //visual studio needs fully qualified name for fptrs, though not c++ std!!!!!
         while (true) {
             
                 this->performJob();
                 std::this_thread::sleep_for(2000ms);            
         }
         jobThread.join();
     }

     void elevatorControllerThread() {
         std::thread elevatorControllerThread(&Elevator::elevatorController);
         while (true) {
             elevatorController();
             std::this_thread::sleep_for(2000ms);
         }
         elevatorControllerThread.join();
     }

     void checkSequence(Call *tempCall)  {

        // Here we are looking for the carCall of the current floorCall
        if (tempCall->getType() == 1 && tempCall->getFloor() == this->currentFloor) {

            int removeIndex = 0;
            bool foundCarCall = false;

            // Traverse carFloors array to look for a
            // carCall with the same ID as tempCall
            for (int i = 0; i < sizeof(this->carCalls); ++i) {
                
                //advance each id and check
                auto tempCarCall = carCalls.begin();
                std::advance(tempCarCall, i);

                if (tempCarCall->getID() == (tempCall->getID())) {

                    removeIndex = i;

                    // Assign passage to carCall
                    // Same direction and higher than currentFloor - P1
                    // Opposite direction - P2

                    if (this->direction == 1) {
                        if ((tempCarCall->getFloor() > this->currentFloor) && (tempCarCall->getDirection() == this->direction)) {
                            tempCarCall->setPassage(1);
                        }
                        else {
                            tempCarCall->setPassage(2);
                        }
                    }
                    else {
                        if ((tempCarCall->getFloor() < this->currentFloor) && (tempCarCall->getDirection() == this->direction)) {
                            tempCarCall->setPassage(1);
                        }
                        else {
                            tempCarCall->setPassage(2);
                        }
                    }

                    //// Add carCall to sequence
                    this->sequence->push_back(*tempCarCall);

                    foundCarCall = true;
                    break;
                }
            }

            // Remove carCall from carCalls array
            if (foundCarCall) {
                 //TODO : remove from list 
            }
        }

        // Check the Calls in the sequence, if the sequence is not empty
        // Here we are looking for all carCalls and floorCalls that can be removed from sequence
        if (sizeof(this->sequence) > 0) {

            // Traverse the Calls in the sequence to find out if
            // any Calls need to be remove, because their floor matches the currentFloor of the elevator
            
            for (std::deque<Call>::iterator it = this->sequence->begin(); it != this->sequence->end(); ++it) {
                // Remove all carCalls whose floor is the current floor of the elevator
                // The passengers whose carCall is the same as currentFloor have already arrived
                if (it->getType() == 0 && it->getFloor() == this->currentFloor) {
                   // this->sequence.remove(call);
                }

                // Remove all floorCalls whose floor is the current floor of the elevator,
                // and add carCalls with the same ID to the sequence
                // The passengers whose floorCall is the same as currentFloor have boarded the elevator
                // and pressed a button inside the elevator (made a carCall)
                if (it->getType() == 1 && it->getFloor() == this->currentFloor) {

                    int removeIndex = 0;
                    bool foundCarCall = false;

                    // Traverse carFloors array

                    for (int i = 0; i < sizeof(this->carCalls); ++i) {
                        //advance each id and check
                        auto tempCarCall = carCalls.begin();
                        std::advance(tempCarCall, i);

                       // Call tempCarCall = this.carCalls.get(i);

                        if (tempCarCall->getID() == (it->getID())) { //call (original)

                            removeIndex = i;

                            // Assign passage to carCall
                            // Same direction and higher than currentFloor - P1
                            // Opposite direction - P2

                            if (this->direction == 1) {
                                if ((tempCarCall->getFloor() > this->currentFloor) && (tempCarCall->getDirection() == this->direction)) {
                                    tempCarCall->setPassage(1);
                                }
                                else {
                                    tempCarCall->setPassage(2);
                                }
                            }
                            else {
                                if ((tempCarCall->getFloor() < this->currentFloor) && (tempCarCall->getDirection() == this->direction)) {
                                    tempCarCall->setPassage(1);
                                }
                                else {
                                    tempCarCall->setPassage(2);
                                }
                            }

                            // Add carCall to sequence
                            this->sequence->push_back(*tempCarCall);
                            foundCarCall = true;
                            break;
                        }
                    }

                    // Remove carCall from carCalls 
                    if (foundCarCall) {
                        this->carCalls.pop_back();
                    }

                    // Remove the floorCall from the sequence
                    this->sequence->pop_back();
                }

            }
        }
    }

    /**
     * Assigns passage to calls in the sequence
     */
     void redefinePassage() {

       // for (Call *tempCall : *sequence) {
        for (std::deque<Call>::iterator it = this->sequence->begin(); it != this->sequence->end(); ++it) {

            if (!it->isSpecialCall()) {

                // Same direction and higher than currentFloor - P1
                // Opposite direction - P2
                // Same direction and lower than currentFloor - P3
                if (this->direction == 1) {
                    if ((it->getFloor() > this->currentFloor) && (it->getDirection() == this->direction)) {
                        it->setPassage(1);
                    }
                    else if ((it->getFloor() < this->currentFloor) && (it->getDirection() == this->direction)) {
                        it->setPassage(3);
                    }
                    else {
                        it->setPassage(2);
                    }
                }
                else {

                    // Same direction and lower than currentFloor - P1
                    // Opposite direction - P2
                    // Same direction and higher than currentFloor - P3
                    if ((it->getFloor() < this->currentFloor) && (it->getDirection() == this->direction)) {
                        it->setPassage(1);
                    }
                    else if ((it->getFloor() > this->currentFloor) && (it->getDirection() == this->direction)) {
                        it->setPassage(3);
                    }
                    else {
                        it->setPassage(2);
                    }
                }
            }
        }
    }

    /**
     * Displays the current position of the elevator in a graphical way.
     */
     void displayElevator() {

        std::printf("\n\nElevator %d\n", this->ID);
        std::printf("------------------------------------------\n");
        for (int i = 0; i < N; ++i) {

            if (i == this->currentFloor) {
                std::printf(" == ");
            }
            else {
                std::printf(" %d ", i);
            }
        }

        if (this->direction == 1) {
            std::printf("\n\n-->");
        }
        else {
            std::printf("\n\n<--");
        }
        std::printf("------------------------------------------\n\n");

    }

    /**
     * Simulates the elevator moving 
     */
    private :
        void performJob()  {

        if (sizeof(this->sequence) > 0) {

            // Get Call from sequence
            Call tempCall = this->sequence->at(0);

            if (tempCall.getFloor() == this->currentFloor) {
                checkSequence(&tempCall);
            }
            else {

                // Update the direction of the elevator based
                // on the position of the current floor
                // Since the direction has changed, we must
                // reassign passage to all calls in the sequence
                if (tempCall.getFloor() < this->currentFloor) {
                    this->direction = 0;
                    redefinePassage();
                }
                else if (tempCall.getFloor() > this->currentFloor) {
                    this->direction = 1;
                    redefinePassage();
                }

                // Simulate elevator movement through the floors of the building
                while ((this->currentFloor != tempCall.getFloor()) &&
                    (this->currentFloor >= 0) &&
                    (this->currentFloor <= (this->N - 1))) {

                    this->idle = false;

                    // Direction is up
                    if (this->direction == 1 && this->currentFloor != (this->N - 1)) {

                        this->currentFloor += 1;
                        //Thread.sleep(this.velocity * this.interFloorHeight * 1000);
                        std::this_thread::sleep_for(2000ms);

                        /*if (DEBUG) {
                            std::printf("\n\n+++++ Elevator %d, direction: %d, current floor: %d, target floor: %d. +++++\n", this->ID, this->direction, this->currentFloor, tempCall.getFloor());
                            std::printf("+++++ Call direction: %d, Call passage: %d, Call floor: %d, Call type: %d, Call ID: %s. +++++\n\n", tempCall.getDirection(), tempCall.getPassage(), tempCall.getFloor(), tempCall.getType(), tempCall.getID());
                        }*/

                        checkSequence(&tempCall);

                        if (!DEBUG) {
                            displayElevator();
                        }

                    }
                    else if (this->direction == 0 && this->currentFloor != 0) {

                        this->currentFloor -= 1;
                       
                        //sleep
                        std::this_thread::sleep_for(2000ms);
                        /*if (DEBUG) {
                            std::printf("\n\n+++++ Elevator %d, direction:%d, current floor: %d, target floor: %d. +++++\n", this->ID, this->direction, this->currentFloor, tempCall.getFloor());
                            std::printf("+++++ Call direction: %d, Call passage: %d, Call floor: %d, Call type: %d, Call ID: %s. +++++\n\n", tempCall.getDirection(), tempCall.getPassage(), tempCall.getFloor(), tempCall.getType(), tempCall.getID());
                        }*/

                        checkSequence(&tempCall);

                        if (!DEBUG) {
                            displayElevator();
                        }

                    }
                    else {
                        //std::printf("\n\n\n\n! + ! + ! Elevator is out of range - performJob() ! + ! + !\n\n\n\n");
                        
                    }
                }
            }

            this->idle = true;
        }
    }

    /**
     * Breaks apart the Passenger object.
     * Puts Passenger.floorCall to the floorCalls array.
     * Puts Passenger.carCall to the carCalls array.
     */
    public :
        void receiveJob(Passenger *temp)  {

        Call floorCall = temp->getFloorCall(); // Has floor, needs passage
        Call carCall = temp->getCarCall(); // Has floor, needs passage

       // this->floorCalls.push_back(floorCall);
        this->carCalls.push_back(carCall);

        /*if (DEBUG) {
            std::printf("--------------------------");
            for (Call call : sequence) {
                std::printf("Call direction: %d, Call passage: %d, Call floor: %d, Call type: %d, Call ID: %s.\n", call.getDirection(), call.getPassage(), call.getFloor(), call.getType(), call.getID());
            }
            std::printf("--------------------------");*/
        
        }
    //}

    /**
     * Responsible for sorting calls assigned by the GroupElevatorController
     * into the elevator’s internal sequence list.
     */
     void elevatorController()  {

        if (sizeof(this->floorCalls) > 0) {
            //go to the begining and get the first item
            auto elevatorFront = this->floorCalls.begin();
            std::advance(elevatorFront, 0); //required to satisfy compiler reequirement

            Call *tempCall = new Call(); // TODO : get floorCall
            //call processed, removw
            this->floorCalls.pop_front();
           
            // Assign passage to a newly arrived floorCall

            // Same direction and higher than currentFloor - P1
            // Opposite direction - P2
            // Same direction and lower than currentFloor - P3
            if (this->direction == 1) {
                if ((tempCall->getFloor() > this->currentFloor) && (tempCall->getDirection() == this->direction)) {
                    tempCall->setPassage(1);
                }
                else if ((tempCall->getFloor() < this->currentFloor) && (tempCall->getDirection() == this->direction)) {
                    tempCall->setPassage(3);
                }
                else {
                    tempCall->setPassage(2);
                }
            }
            else {

                // Same direction and lower than currentFloor - P1
                // Opposite direction - P2
                // Same direction and higher than currentFloor - P3
                if ((tempCall->getFloor() < this->currentFloor) && (tempCall->getDirection() == this->direction)) {
                    tempCall->setPassage(1);
                }
                else if ((tempCall->getFloor() > this->currentFloor) && (tempCall->getDirection() == this->direction)) {
                    tempCall->setPassage(3);
                }
                else {
                    tempCall->setPassage(2);
                }
            }
            //push back
            this->sequence->push_back(*tempCall);
        }
    }
};


