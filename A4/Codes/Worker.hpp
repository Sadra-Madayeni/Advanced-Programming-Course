#ifndef WORKER_HPP
#define WORKER_HPP

#include "Car.hpp"

class Worker {
public:
    Worker(int id, int stageId, int timeToFinish);

    // Getters
    int getId() const;
    int getStageId() const;
    int getTimeToFinish() const;  
    bool isBusy() const;
    int getCurrentCarId() const;  

 
    void assignCar(Car* car); 
    Car* releaseCar();       
    void decreaseTime(int timeUnits);  
    int getRemainingTime() const;

 
    bool isBetterThan(const Worker* other) const;

private:
    int id;
    int stageId;
    int timeToFinish; 
    
 
    Car* currentCar;  
    int remainingTime;  
};

#endif