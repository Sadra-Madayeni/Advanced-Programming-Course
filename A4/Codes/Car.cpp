#include "Car.hpp"

Car::Car(int id, const std::vector<int>& stages) 
    : id(id), plannedStages(stages), currentStageIndex(0), status(CarStatus::Arrived) {}

int Car::getId() const {
    return id;
}

CarStatus Car::getStatus() const {
    return status;
}

int Car::getCurrentStageId() const {
    if (isFinished()) return -1;
    return plannedStages[currentStageIndex];
}

bool Car::isFinished() const {
    return currentStageIndex >= plannedStages.size();
}

void Car::setStatus(CarStatus newStatus) {
    status = newStatus;
}

void Car::advanceStage() {
    if (!isFinished()) {
        currentStageIndex++;
    }
}

int Car::getNextPlannedStageId() const {
    if (currentStageIndex + 1 >= plannedStages.size()) {
        return -1; 
    }
    return plannedStages[currentStageIndex + 1];
}