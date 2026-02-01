#include "Stage.hpp"
#include <algorithm>

using namespace std;

Stage::Stage(int id, int price)
    : id(id), price(price), totalIncome(0), totalWashedCars(0) {}

void Stage::addWorker(Worker* worker) {
    workers.push_back(worker);
}

void Stage::addCarToQueue(Car* car) {
    carQueue.push_back(car);
    car->setStatus(CarStatus::InLine);
}

 vector<Car*> Stage::assignFreeWorkers() {
     vector<Car*> assignedCars;

    if (carQueue.empty()) {
        return assignedCars;
    }

     vector<Worker*> freeWorkers;
    for (Worker* w : workers) {
        if (!w->isBusy()) {
            freeWorkers.push_back(w);
        }
    }

    if (freeWorkers.empty()) {
        return assignedCars;
    }


     sort(freeWorkers.begin(), freeWorkers.end(), 
              [](Worker* a, Worker* b) {
                  return a->isBetterThan(b);
              });

    while (!carQueue.empty() && !freeWorkers.empty()) {
        Car* car = carQueue.front();
        carQueue.pop_front();

        Worker* bestWorker = freeWorkers.front();
        freeWorkers.erase(freeWorkers.begin()); 

        // انجام تخصیص
        bestWorker->assignCar(car);
        car->setStatus(CarStatus::InService);
        
        assignedCars.push_back(car); 
    }

    return assignedCars;
}

void Stage::addToIncome() {
    totalIncome += price;
    totalWashedCars++;
}

int Stage::getId() const { return id; }
int Stage::getPrice() const { return price; }
long long Stage::getIncome() const { return totalIncome; }
int Stage::getWashedCount() const { return totalWashedCars; }
int Stage::getQueueSize() const { return carQueue.size(); }

int Stage::getWorkingCount() const {
    int count = 0;
    for (const auto& w : workers) {
        if (w->isBusy()) count++;
    }
    return count;
}