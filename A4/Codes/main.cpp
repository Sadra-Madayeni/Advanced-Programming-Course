// main.cpp
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "CarWashManager.hpp"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        return 1;
    }

    CarWashManager manager(argv[1], argv[2]);

    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        string command;
        ss >> command;

        if (command == "pass_time") {
            int time;
            ss >> time;
            manager.passTime(time);
        } 
        else if (command == "car_arrival") {
            vector<int> stages;
            int stageId;
            while (ss >> stageId) {
                stages.push_back(stageId);
            }
            manager.handleCarArrival(stages);
        }
        else if (command == "get_stage_status") {
            int id;
            ss >> id;
            manager.printStageStatus(id);
        }
        else if (command == "get_worker_status") {
            int id;
            ss >> id;
            manager.printWorkerStatus(id);
        }
        else if (command == "get_car_status") {
            int id;
            ss >> id;
            manager.printCarStatus(id);
        }
    }

    return 0;
}