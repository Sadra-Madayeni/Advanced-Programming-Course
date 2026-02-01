// CarWashManager.hpp
#ifndef CARWASHMANAGER_HPP
#define CARWASHMANAGER_HPP

#include <vector>
#include <string>
#include <iostream>
#include "Stage.hpp"
#include "Worker.hpp"
#include "Car.hpp"

class CarWashManager {
public:
    CarWashManager(const std::string& stagesPath, const std::string& workersPath);
    ~CarWashManager(); 

  
    void passTime(int timeUnits);           
    void handleCarArrival(const std::vector<int>& stages); 
    void printStageStatus(int stageId) const; 
    void printWorkerStatus(int workerId) const;
    void printCarStatus(int carId) const;   

private:
    int currentTime;
    int nextCarId; 

    std::vector<Stage*> stages;
    std::vector<Worker*> workers;
    std::vector<Car*> cars;

    void loadStages(const std::string& path);
    void loadWorkers(const std::string& path);

    Stage* getStageById(int id) const;
    Worker* getWorkerById(int id) const;
    Car* getCarById(int id) const;

    void processOneTimeUnit(); 
};

#endif