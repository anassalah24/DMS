#include "dmsmanager.h"

DMSManager::DMSManager(ThreadSafeQueue<cv::Mat>& cameraQueue, ThreadSafeQueue<cv::Mat>& preprocessingQueue, ThreadSafeQueue<cv::Mat>& faceDetectionQueue,ThreadSafeQueue<cv::Mat>& tcpOutputQueue, int tcpPort , ThreadSafeQueue<CarState>& stateOutputQueue , ThreadSafeQueue<int>& postOutputQueue )
: cameraComponent(cameraQueue), preprocessingComponent(cameraQueue, preprocessingQueue), faceDetectionComponent(cameraQueue, faceDetectionQueue), tcpComponent(tcpPort, preprocessingQueue),vehicleStateManager(stateOutputQueue),postProcessingComponent(stateOutputQueue, postOutputQueue), cameraQueue(cameraQueue), preprocessingQueue(preprocessingQueue), faceDetectionQueue(faceDetectionQueue), tcpPort(tcpPort),stateOutputQueue(stateOutputQueue),postOutputQueue(postOutputQueue),running(false) {}//change back the tcp queue

DMSManager::~DMSManager() {
    stopSystem();  // Ensure all components and threads are stopped and cleaned up properly
}

bool DMSManager::startSystem() {
    if (running) return false;  // Prevent the system from starting if it's already running

    running = true;

    cameraThread = std::thread(&DMSManager::cameraLoop, this);  // Start the camera loop in its own thread
    preprocessingThread = std::thread(&DMSManager::preprocessingLoop, this);  // Start the preprocessing loop in its own thread
    //faceDetectionThread = std::thread(&DMSManager::faceDetectionLoop, this);  // Start face detection in its own thread
    tcpThread = std::thread(&DMSManager::commtcpLoop, this);
    
    vehicleStateThread = std::thread(&DMSManager::vehicleStateLoop, this);
    postProcessingThread = std::thread(&DMSManager::postprocessingLoop, this);
    return true;
}

void DMSManager::stopSystem() {
    running = false;  // Signal all loops to stop

    if (cameraThread.joinable()) cameraThread.join();
    //if (preprocessingThread.joinable()) preprocessingThread.join();
    if (faceDetectionThread.joinable()) faceDetectionThread.join();  // Ensure the face detection thread is joined
    tcpComponent.stopServer();  // Stop the TCP server

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
void DMSManager::commtcpLoop() {
    tcpComponent.startServer();
}

void DMSManager::vehicleStateLoop() {
    vehicleStateManager.startStateManager();
}

void DMSManager::postprocessingLoop(){
    postProcessingComponent.postProcess();
}




bool DMSManager::initializeCamera(const std::string& source) {
    return cameraComponent.initialize(source);
}

bool DMSManager::initializeFaceDetection(const std::string& modelConfiguration, const std::string& modelWeights) {
    return faceDetectionComponent.initialize(modelConfiguration, modelWeights);
}

// bool DMSManager::initializeVehicleState(const std::string& steeringFilePath, const std::string& velocityFilePath) {
//     vehicleStateManager.parseCarState(steeringFilePath);
// Implement any additional methods required for managing the TCP component

