#include "dmsmanager.h"

DMSManager::DMSManager(ThreadSafeQueue<cv::Mat>& cameraQueue, ThreadSafeQueue<cv::Mat>& preprocessingQueue, ThreadSafeQueue<cv::Mat>& faceDetectionQueue)
: cameraComponent(cameraQueue), preprocessingComponent(cameraQueue, preprocessingQueue), faceDetectionComponent(cameraQueue, faceDetectionQueue), cameraQueue(cameraQueue), preprocessingQueue(preprocessingQueue),faceDetectionQueue(faceDetectionQueue), running(false) {}


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

    faceDetectionThread = std::thread(&DMSManager::faceDetectionLoop, this);  // Start face detection in its own thread

    return true;
}

void DMSManager::stopSystem() {
    running = false; // Signal all loops to stop

    // Wake up threads if they are waiting on condition variables (if any used in your component loops)
    
    if (cameraThread.joinable()) cameraThread.join();
    if (preprocessingThread.joinable()) preprocessingThread.join();
    if (faceDetectionThread.joinable()) faceDetectionThread.join();  // Ensure the face detection thread is joined

    // Additional cleanup if necessary
}

void DMSManager::cameraLoop() {
    cameraComponent.startCapture();
}

void DMSManager::preprocessingLoop() {
    preprocessingComponent.startProcessing();
}
void DMSManager::faceDetectionLoop() {
    faceDetectionComponent.startDetection();  // This should start the internal loop of the face detection component
}

bool DMSManager::initializeCamera(const std::string& source) {
    return cameraComponent.initialize(source);
}

bool DMSManager::initializeFaceDetection(const std::string& modelConfiguration, const std::string& modelWeights) {
    return faceDetectionComponent.initialize(modelConfiguration, modelWeights);
}



