#include "dmsmanager.h"

DMSManager::DMSManager(ThreadSafeQueue<cv::Mat>& cameraQueue, ThreadSafeQueue<cv::Mat>& preprocessingQueue) 
: cameraComponent(cameraQueue), preprocessingComponent(cameraQueue, preprocessingQueue), cameraQueue(cameraQueue), preprocessingQueue(preprocessingQueue), running(false) {}

DMSManager::~DMSManager() {
    stopSystem(); // Ensure all components and threads are stopped and cleaned up properly
}

bool DMSManager::startSystem() {
    if (running) return false; // Prevent the system from starting if it's already running

    running = true;

    // Start the camera loop in its own thread
    cameraThread = std::thread(&DMSManager::cameraLoop, this);

    // Start the preprocessing loop in its own thread
    preprocessingThread = std::thread(&DMSManager::preprocessingLoop, this);

    return true;
}

void DMSManager::stopSystem() {
    running = false; // Signal all loops to stop

    // Wake up threads if they are waiting on condition variables (if any used in your component loops)
    
    if (cameraThread.joinable()) cameraThread.join();
    if (preprocessingThread.joinable()) preprocessingThread.join();

    // Additional cleanup if necessary
}

void DMSManager::cameraLoop() {
    cameraComponent.startCapture();
}

void DMSManager::preprocessingLoop() {
    preprocessingComponent.startProcessing();
}
bool DMSManager::initializeCamera(const std::string& source) {
    return cameraComponent.initialize(source);
}


