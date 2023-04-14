#pragma once
#include "Elevator.h"
#include  "Floor.h"
#include "RoundRobin.h"

 class GroupElevatorController  {

 private :
     Elevator *elevatorGroup[2] ;
     Floor *floors[5];

     int algorithm;
     int start;

     int N; // Number of floors
     int L; // Number of elevators
     int U; // Building population

     RoundRobin *roundRobin;
 
 public :
     GroupElevatorController(Elevator elevator[], Floor floor[]) {
      
        this->start = 0;
        this->roundRobin = new RoundRobin();
        //TODO : implement other algo - viz. zoning, threepassage     
    
        //TODO : add getters and setters      

        while (true) {
            scheduler();
            //Thread.sleep(2000);
        }  
    }

    /**
     * Scan the floors array, looks for a floor with at least one passenger.
     * Based on the algorithm, assigns a Passenger to one of the elevators
     * from the elevatorGroup array.
     */
     void scheduler()  {

        int chosenElevator = 0;
        bool foundPassenger = false;

        Passenger *tempPassenger = nullptr; // Create a dummy Passenger object

        // Look for a floor with at least one passenger
        for (int i = this->start; i < sizeof(floors); ++i) {

            Floor *floor = floors[i];

            if (sizeof(floor->getPassengers()) > 0) {

                 floor->getPassengers().pop_back(); // Remove the passenger 

                // Remembers from which index to start scanning next time
                if (i == 4) { //sizeof(floor -1)
                    this->start = 0;
                }
                else {
                    this->start = i + 1;
                }

                foundPassenger = true;
                break;
            }

            // Remembers from which index to start scanning next time
            // even though no passenger was found
            if (i == 4) { //building has 5 floors, index starts with 0 (5 -1=4)
                this->start = 0;
            }
            else {
                this->start = i + 1;
            }
        }

        if (foundPassenger) {

            // Each algorithm returns the index of the chosen elevator
            // The chosen elevator will be given a task (receive job)
            switch (this->algorithm) {
            case 1:
                chosenElevator = roundRobin->choseElevator(*elevatorGroup, this->L);
                break;
            default:
                chosenElevator = roundRobin->choseElevator(*elevatorGroup, this->L);
                break;
            }

            this->elevatorGroup[chosenElevator]->receiveJob(tempPassenger); // Assign a passenger to an elevator
        }

    }
};

