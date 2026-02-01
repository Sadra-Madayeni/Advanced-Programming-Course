#include "Worker.hpp"

Worker::Worker(int id, int stageId, int timeToFinish)
    : id(id), stageId(stageId), timeToFinish(timeToFinish), 
      currentCar(nullptr), remainingTime(0) {}

int Worker::getId() const {
    return id;
}

int Worker::getStageId() const {
    return stageId;
}

int Worker::getTimeToFinish() const {
    return timeToFinish;
}

bool Worker::isBusy() const {
    return currentCar != nullptr;
}

int Worker::getCurrentCarId() const {
    if (currentCar) {
        return currentCar->getId();
    }
    return -1;
}

void Worker::assignCar(Car* car) {
    currentCar = car;
    remainingTime = timeToFinish;
}

Car* Worker::releaseCar() {
    Car* car = currentCar;
    currentCar = nullptr;
    remainingTime = 0;
    return car;
}

void Worker::decreaseTime(int timeUnits) {
    if (isBusy()) {
        remainingTime -= timeUnits;
        if (remainingTime < 0) remainingTime = 0;
    }
}

int Worker::getRemainingTime() const {
    return remainingTime;
}

bool Worker::isBetterThan(const Worker* other) const {
 
    if (this->timeToFinish != other->timeToFinish) {
        return this->timeToFinish < other->timeToFinish;
    }
 
    return this->id < other->id;
}