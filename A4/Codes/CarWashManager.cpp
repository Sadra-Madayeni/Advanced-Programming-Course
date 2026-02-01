#include "CarWashManager.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;


struct EventLog {
    int carId;
    string message;

    bool operator<(const EventLog& other) const {
        return carId < other.carId;
    }
};


vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    if (!s.empty() && s.back() == delimiter) tokens.push_back("");
    return tokens;
}


CarWashManager::CarWashManager(const string& stagesPath, const string& workersPath) 
    : currentTime(0), nextCarId(1) {
    loadStages(stagesPath);
    loadWorkers(workersPath);
}

CarWashManager::~CarWashManager() {
    for (auto s : stages) delete s;
    for (auto w : workers) delete w;
    for (auto c : cars) delete c;
}



void CarWashManager::loadStages(const string& path) {
    ifstream file(path);
    string line;
    // رد کردن هدر
    if (file.good()) getline(file, line);

    while (getline(file, line)) {
        string cleanLine = line;
        // حذف کاراکترهای اضافی احتمالی مثل \r
        cleanLine.erase(remove(cleanLine.begin(), cleanLine.end(), '\r'), cleanLine.end());
        
        if (cleanLine.empty()) continue;
        
        vector<string> parts = split(cleanLine, ',');
        if (parts.size() >= 2) {
            int id = stoi(parts[0]);
            int price = stoi(parts[1]);
            stages.push_back(new Stage(id, price));
        }
    }
}

void CarWashManager::loadWorkers(const string& path) {
    ifstream file(path);
    string line;

    if (file.good()) getline(file, line);

    while (getline(file, line)) {
        string cleanLine = line;
        cleanLine.erase(remove(cleanLine.begin(), cleanLine.end(), '\r'), cleanLine.end());

        if (cleanLine.empty()) continue;
        
        vector<string> parts = split(cleanLine, ',');
        if (parts.size() >= 3) {
            int id = stoi(parts[0]);
            int stageId = stoi(parts[1]);
            int timeToFinish = stoi(parts[2]);
            
            Worker* newWorker = new Worker(id, stageId, timeToFinish);
            workers.push_back(newWorker);
            
            // اضافه کردن کارگر به مرحله مربوطه
            Stage* s = getStageById(stageId);
            if (s) {
                s->addWorker(newWorker);
            }
        }
    }
}



Stage* CarWashManager::getStageById(int id) const {
    for (auto s : stages) {
        if (s->getId() == id) return s;
    }
    return nullptr;
}

Worker* CarWashManager::getWorkerById(int id) const {
    for (auto w : workers) {
        if (w->getId() == id) return w;
    }
    return nullptr;
}

Car* CarWashManager::getCarById(int id) const {
    for (auto c : cars) {
        if (c->getId() == id) return c;
    }
    return nullptr;
}


void tryEnterStage(Car* car, Stage* nextStage, int currentTime, vector<EventLog>& logs, const string& prevLocation) {
    nextStage->addCarToQueue(car); 
    
    vector<Car*> startedCars = nextStage->assignFreeWorkers();
    
    bool startedImmediately = false;
    for (auto c : startedCars) {
        if (c->getId() == car->getId()) {
            startedImmediately = true;
            break;
        }
    }

    string msg = to_string(currentTime) + " Car " + to_string(car->getId()) + ": " + prevLocation + " -> ";
    if (startedImmediately) {
        msg += "Stage " + to_string(nextStage->getId());
    } else {
        msg += "Queue " + to_string(nextStage->getId());
    }
    logs.push_back({car->getId(), msg});
}


void CarWashManager::passTime(int timeUnits) {
    for (int i = 0; i < timeUnits; ++i) {
        currentTime++; 
        processOneTimeUnit();
    }
}

void CarWashManager::processOneTimeUnit() {
    vector<EventLog> logs;
    vector<Car*> finishedCars; 


    for (auto w : workers) {
        if (w->isBusy()) {
            w->decreaseTime(1);
            if (w->getRemainingTime() == 0) {
                Car* car = w->releaseCar();
                finishedCars.push_back(car);
                
                Stage* s = getStageById(w->getStageId());
                if (s) s->addToIncome();
            }
        }
    }


    sort(finishedCars.begin(), finishedCars.end(), [](Car* a, Car* b) {
        return a->getId() < b->getId();
    });

 
    for (auto car : finishedCars) {
        int finishedStageId = car->getCurrentStageId();
        string prevLoc = "Stage " + to_string(finishedStageId);
        
        car->advanceStage(); 

        if (car->isFinished()) {
            car->setStatus(CarStatus::Done);
            string msg = to_string(currentTime) + " Car " + to_string(car->getId()) + ": " + prevLoc + " -> Done";
            logs.push_back({car->getId(), msg});
        } else {
            int nextStageId = car->getCurrentStageId();
            Stage* nextStage = getStageById(nextStageId);
            if (nextStage) {
                tryEnterStage(car, nextStage, currentTime, logs, prevLoc);
            }
        }
    }

  
    for (auto s : stages) {
        vector<Car*> fromQueue = s->assignFreeWorkers();
        for (auto car : fromQueue) {
            bool alreadyLogged = false;
            for (const auto& log : logs) {
                if (log.carId == car->getId()) {
                    alreadyLogged = true;
                    break;
                }
            }

            if (!alreadyLogged) {
                string msg = to_string(currentTime) + " Car " + to_string(car->getId()) + ": Queue " + to_string(s->getId()) + " -> Stage " + to_string(s->getId());
                logs.push_back({car->getId(), msg});
            }
        }
    }


    sort(logs.begin(), logs.end());
    for (const auto& log : logs) {
        cout << log.message << endl;
    }
}

void CarWashManager::handleCarArrival(const vector<int>& stageIds) {
    Car* newCar = new Car(nextCarId++, stageIds);
    cars.push_back(newCar);

    int firstStageId = newCar->getCurrentStageId();
    Stage* firstStage = getStageById(firstStageId);
    
    if (firstStage) {
        vector<EventLog> tempLogs;
        tryEnterStage(newCar, firstStage, currentTime, tempLogs, "Arrived"); 
        for(const auto& l : tempLogs) cout << l.message << endl;
    }
}


void CarWashManager::printStageStatus(int stageId) const {
    Stage* s = getStageById(stageId);
    if (!s) {
        cout << "NOT FOUND" << endl;
        return;
    }
    cout << "Number of washed cars: " << s->getWashedCount() << endl;
    cout << "Number of cars in queue: " << s->getQueueSize() << endl;
    cout << "Number of cars being washed: " << s->getWorkingCount() << endl;
    cout << "Income: " << s->getIncome() << endl;
}

void CarWashManager::printWorkerStatus(int workerId) const {
    Worker* w = getWorkerById(workerId);
    if (!w) {
        cout << "NOT FOUND" << endl;
        return;
    }
    if (w->isBusy()) {
        cout << "Working: " << w->getCurrentCarId() << endl;
    } else {
        cout << "Idle" << endl;
    }
}

void CarWashManager::printCarStatus(int carId) const {
    Car* c = getCarById(carId);
    if (!c) {
        cout << "NOT FOUND" << endl;
        return;
    }
    switch (c->getStatus()) {
        case CarStatus::Arrived:
            cout << "Arrived" << endl; 
            break;
        case CarStatus::InLine:
            cout << "In line: " << c->getCurrentStageId() << endl;
            break;
        case CarStatus::InService:
            cout << "In service: " << c->getCurrentStageId() << endl;
            break;
        case CarStatus::Done:
            cout << "Done" << endl;
            break;
    }
}