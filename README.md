# MultiElevator Scheduling Simulation
MultiElevator Scheduling Simulation 

Elevator simulation environment, which mimics the scheduling of multiple elevators within a building of any size.
The Current version supports 2 elevator cars and the building has 5 floors.
 
 The multithreaded approach helps to simulate elevators running concurrently. Design supports Ts use of several group scheduling algorithms, which help to distribute the workload between elevator-cars as well as make the elevators adaptable to various situations (eg. more people in the morning - Up-Peak, everyone is going to the main floor - Zoning).
The Current version supports round-robin.

Due to time constraints, the application wasn't thoroughly tested, and my have bugs.

#Data Structures Used

DeQue :
Allows for custom element ordering
The data structure was used to implement sequence, in which the calls had to be sorted based on their passage

MultiThreading :
To mimic calls in async manner

Group Scheduling Algorithms Implemented
Round-Robin

Is the default group scheduling algorithm
Calls are assigned in the order they arrive in a sequential way to single elevators

Building Thread
The main thread
Randomly chooses a floor from the floors array and calls the generatePassenger() method
Sleep time is static

GroupElevatorController Thread
The thread calls the scheduler() method with constant interval 
The scheduler() method scans the floors array and looks for passengers
If a passenger is found, then a job is assign to one of the elevators from the elevatorGroup array
The target elevator is chosen by group scheduling algorithms

Elevator Thread  I
The elevatorControllerThread calls the elevatorController() method with static interval 
The elevatorController() method:
Gets a floorCall from floorCalls array
Assigns it a passage
Adds it to the sequence queue

Elevator Thread II
The performJobThread calls the performJob() method with static interval 
The performJob() method:
Takes a call from sequence queue
Simulates the elevator going up or down

Elevator Thread III
The upPeakThread relocates elevators in idle state to the lobby.

Elevator Thread IV
The zoningThread relocates elevators in idle state to go to a floor within their assigned zone.

What's Next
Analyze each algorithm by running the system with different inputs:
Different number of elevators
Different number of requests
Implement the functionality to switch dynamically between the algorithms during runtime
Implemented disk scheduling algorithms:
First Come-First Serve (FCFS)
Shortest Seek Time First (SSTF)
Elevator (SCAN)
Circular SCAN (C-SCAN)
Use Boost for STL functions
Improve car holing person logic
LOOK
C-LOOK
Add unit test (google tests)

References

https://www.geeksforgeeks.org/disk-scheduling-algorithms/

https://www.geeksforgeeks.org/scan-elevator-disk-scheduling-algorithms/

https://www.programiz.com/cpp-programming/queue

https://github.com/00111000/Elevator-Scheduling-Simulator/tree/8c1275d00424954a452b58a41b8dae14cb8d2ebb/research-papers
[
](https://www.geeksforgeeks.org/scan-elevator-disk-scheduling-algorithms/)

