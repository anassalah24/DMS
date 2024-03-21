#pragma once


#include <string>
#include <thread>
#include "threadsafequeue.h"


// Define structure for input data
struct CarState {
    double steeringWheelAngle;
    double velocity;
    bool blinkersOn;
};

class VehicleStateManager {
public:
    VehicleStateManager(ThreadSafeQueue<CarState>& outputQueue);
    ~VehicleStateManager();

    void startStateManager();

    void parseCarState(const std::string& dataFilePath);
    double extractValueFromLine(const std::string& line, const std::string& keyword);

    // Getters for CarState might be useful for accessing the parsed data
    CarState getCarState() const;

private:
    CarState state;
    std::thread stateThread;
    bool running;
    ThreadSafeQueue<CarState>& outputQueue;
    void stateLoop();

};

