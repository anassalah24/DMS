#include <thread>
#include "threadsafequeue.h"
#include "basiccameracomponent.h"
#include "basicpreprocessingcomponent.h"
#include "facedetectioncomponent.h"
#include "commtcpcomponent.h"  // Include the CommTCPComponent header
#include "vehiclestatemanager.h"
#include "postprocessingcomponent.h"

class DMSManager {
public:
    DMSManager(ThreadSafeQueue<cv::Mat>& cameraQueue, ThreadSafeQueue<cv::Mat>& preprocessingQueue, ThreadSafeQueue<cv::Mat>& faceDetectionQueue,ThreadSafeQueue<cv::Mat>& tcpOutputQueue, int tcpPort, ThreadSafeQueue<CarState>& stateOutputQueue , ThreadSafeQueue<int>& postOutputQueue);
    ~DMSManager();

    bool startSystem();
    void stopSystem();
    bool initializeCamera(const std::string& source);
    bool initializeFaceDetection(const std::string& modelConfiguration, const std::string& modelWeights);
    //bool initializeVehicleState(const std::string& steeringFilePath, const std::string& velocityFilePath);
    bool initializePostProcessing();

private:
    BasicCameraComponent cameraComponent;
    BasicPreprocessingComponent preprocessingComponent;
    FaceDetectionComponent faceDetectionComponent;
    CommTCPComponent tcpComponent;  // Add the CommTCPComponent member variable
    VehicleStateManager vehicleStateManager;
    PostProcessingComponent postProcessingComponent;

    ThreadSafeQueue<cv::Mat>& cameraQueue;
    ThreadSafeQueue<cv::Mat>& preprocessingQueue;
    ThreadSafeQueue<cv::Mat>& faceDetectionQueue;
    ThreadSafeQueue<cv::Mat> tcpOutputQueue;  // Add a queue for frames to send over TCP, if needed
    ThreadSafeQueue<CarState>& stateOutputQueue;
    ThreadSafeQueue<int>& postOutputQueue;


    std::thread cameraThread;
    std::thread preprocessingThread;
    std::thread faceDetectionThread;
    std::thread tcpThread;  // Add a thread for the TCP component
    std::thread vehicleStateThread; // New, if needed
    std::thread postProcessingThread; 

    int tcpPort;  // Store the TCP port number
    bool running;

    void cameraLoop();
    void preprocessingLoop();
    void faceDetectionLoop();
    void commtcpLoop();  // Add a loop for the TCP component, if needed
    void vehicleStateLoop(); // New, if vehicle state needs continuous update
    void postprocessingLoop(); 
    // Any additional methods for handling TCP communication
};

