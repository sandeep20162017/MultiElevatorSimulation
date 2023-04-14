#pragma once
#include "Elevator.h"
#include "Floor.h"
#include "Elevator.h"
#include "GroupElevatorController.h"

 class Building {

 private :
     int N = 5; // Number of floors
     int L = 2; // Number of elevators
     int U = 20; // Building population
     int algorithm; // Desired algorithm will be passed as a CL arg

     Elevator *elevatorGroup[2]; // An array of L = 2 elevators
     Floor *floors[5]; // An array of N = 5 floors
     GroupElevatorController *controller; // reference to the controller used for controller setup
      

 public :
     Building(int N, int L, int U) {
        this->N = N;
        this->L = L;
        this->U = U;

        this->elevatorGroup[0] = new Elevator[this->L];       
        //TODO : add to controller this->controller = new GroupElevatorController(this->elevatorGroup, this->floors);       
    }

     int getN() {
        return N;
    }

     void setAlgorithm(int algorithm) {
        this->algorithm = algorithm;
    }

    /**
     * Creates L=2 Elevator objects in the elevatorGroup array.
     */
    void createElevators() {
        for (int i = 0; i < this->L; ++i) {
            this->elevatorGroup[i] = new Elevator(i, this->algorithm, 1,1, 1,8, 5);
            this->elevatorGroup[i]->setCurrentFloor(N / 2);
            this->elevatorGroup[i]->setDirection(1);
            this->elevatorGroup[i]->setN(N);
            this->elevatorGroup[i]->setL(L);
        }

        // Create elevator threads
        for (int i = 0; i < this->L; ++i) {
            this->elevatorGroup[i]->elevatorControllerThread();
            this->elevatorGroup[i]->performJobThread();

            // Start this thread only if user chose Up-peak
            if (this->algorithm == 2) {
                this->elevatorGroup[i]->upPeakThread();
            }
         
        }
    }

    /**
     * Creates N Floor objects in the floors array.
     */
     void createFloors() {
        for (int i = 0; i < this->N; ++i) {
            Floor *fp = new Floor(i);
            this->floors[i] = fp;
        }
    }

     GroupElevatorController getController() {
        return *controller;
    }

    /**
     * Randomly selects a floor from the floors array and
     * calls the generatePassenger method on the Floor(randFloor) object.
     */
     void generatePassenger(int N)  {
       //just for verification, use rand generator to generate passengers on the floor
        floors[2]->generatePassenger(N);
    }

};

