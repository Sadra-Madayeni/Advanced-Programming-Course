// Stage.hpp
#ifndef STAGE_HPP
#define STAGE_HPP

#include <vector>
#include <deque>
#include "Worker.hpp"
#include "Car.hpp"

using namespace std;

class Stage {
public:
    Stage(int id, int price);

 
    void addWorker(Worker* worker);

    // Operations
    void addCarToQueue(Car* car);  
    
 
     vector<Car*> assignFreeWorkers(); 

 
    void addToIncome(); 
    
 
    int getId() const;
    int getPrice() const;
    long long getIncome() const;
    int getWashedCount() const;
    int getQueueSize() const;
    int getWorkingCount() const;  

private:
    int id;
    int price;
    
     vector<Worker*> workers; 
     deque<Car*> carQueue;   
    
 
    long long totalIncome;
    int totalWashedCars;
};

#endif