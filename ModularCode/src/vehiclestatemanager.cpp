#include "vehiclestatemanager.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

VehicleStateManager::VehicleStateManager(ThreadSafeQueue<CarState>& outputQueue):outputQueue(outputQueue),running(false) {}

VehicleStateManager::~VehicleStateManager() {}

void VehicleStateManager::startStateManager() {
    if (running) {
        std::cerr << "state is already running." << std::endl;
        return;
    }
    running = true;
    stateThread = std::thread(&VehicleStateManager::stateLoop, this);
}

// Helper function to extract a numerical value from a line of text
double VehicleStateManager::extractValueFromLine(const std::string& line, const std::string& keyword) {
    size_t pos = line.find(keyword);
    if (pos != string::npos) {
        size_t number_pos = line.find_first_of("-0123456789", pos + keyword.size());
        if (number_pos != string::npos) {
            try {
                return stod(line.substr(number_pos));
            } catch (const std::invalid_argument& e) {
                cerr << "Invalid number format in line: " << line << endl;
                return 0.0; // or handle error appropriately
            }
        }
    }
    return 0.0; // Indicates not found or error, depending on your error handling strategy
}


void VehicleStateManager::parseCarState(const std::string& dataFilePath) {
    ifstream dataFile(dataFilePath);
    string line;
    if (dataFile.is_open()) {
        while (getline(dataFile, line)) {
            // Attempt to extract the steering wheel angle and velocity from each line
            double extractedValue = extractValueFromLine(line, "steering");
            if (extractedValue != 0.0) { // Assuming 0.0 means not found or invalid
                state.steeringWheelAngle = extractedValue;
                // cout << "Steering wheel angle: " << state.steeringWheelAngle << " degrees" << endl;
            }
            extractedValue = extractValueFromLine(line, "velocity");
            if (extractedValue != 0.0) { // Assuming 0.0 means not found or invalid
                state.velocity = extractedValue;
                // cout << "Car velocity: " << state.velocity << " km/hr" << endl;
            }
        }
        dataFile.close();
    } else {
        cerr << "Unable to open data file: " << dataFilePath << endl;
    }
}

CarState VehicleStateManager::getCarState() const {
    return state;
}

void VehicleStateManager::stateLoop() {
    while (running) {
        // Specify the paths to the files where new data is periodically written
        const std::string FilePath = TEXT_FILE_LOCATION;

        // Call parseCarState to update the vehicle state based on new data
        parseCarState(FilePath);

        // Optionally, after parsing the new state, notify other components that might need this updated data.
        // For example, if the PostProcessingComponent needs the latest vehicle state:
        CarState currentState = getCarState();
        // postProcessingComponent.updateVehicleState(currentState); // You'd need to implement this method.
        outputQueue.push(currentState);
        //CarState debugqueuestate;
        //outputQueue.tryPop(debugqueuestate);
        //std::cout << "Queue vehicle state: Steering angle " << debugqueuestate.steeringWheelAngle << ", Velocity " << debugqueuestate.velocity << std::endl;


        // Log, debug, or perform actions based on the updated state as necessary
        //std::cout << "Updated vehicle state: Steering angle " << currentState.steeringWheelAngle << ", Velocity " << currentState.velocity << std::endl;

        // Wait a bit before the next update to avoid flooding with too many updates per second.
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // Example: update twice per second
        }
}

