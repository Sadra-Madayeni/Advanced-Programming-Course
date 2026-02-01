#ifndef CAR_HPP
#define CAR_HPP

#include <vector>
#include <string>
#include "Types.hpp"

class Car {
public:
    Car(int id, const std::vector<int>& stages);

    // Getters
    int getId() const;
    CarStatus getStatus() const;
    int getCurrentStageId() const; 
    bool isFinished() const;

    // Setters / Actions
    void setStatus(CarStatus newStatus);
    void advanceStage(); 
    int getNextPlannedStageId() const; 

private:
    int id;
    std::vector<int> plannedStages; 
    size_t currentStageIndex;    
    CarStatus status;
};

#endif