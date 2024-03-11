#include <thread>
#include "threadsafequeue.h"
#include "basiccameracomponent.h"
#include "basicpreprocessingcomponent.h"
#include "facedetectioncomponent.h"
#include "commtcpcomponent.h"  // Include the CommTCPComponent header

class DMSManager {
public:
    DMSManager(ThreadSafeQueue<cv::Mat>& cameraQueue, ThreadSafeQueue<cv::Mat>& preprocessingQueue, ThreadSafeQueue<cv::Mat>& faceDetectionQueue,ThreadSafeQueue<cv::Mat>& tcpOutputQueue, int tcpPort);
    ~DMSManager();

    bool startSystem();
    void stopSystem();
    bool initializeCamera(const std::string& source);
    bool initializeFaceDetection(const std::string& modelConfiguration, const std::string& modelWeights);

private:
    BasicCameraComponent cameraComponent;
    BasicPreprocessingComponent preprocessingComponent;
    FaceDetectionComponent faceDetectionComponent;
    CommTCPComponent tcpComponent;  // Add the CommTCPComponent member variable

    ThreadSafeQueue<cv::Mat>& cameraQueue;
    ThreadSafeQueue<cv::Mat>& preprocessingQueue;
    ThreadSafeQueue<cv::Mat>& faceDetectionQueue;
    ThreadSafeQueue<cv::Mat> tcpOutputQueue;  // Add a queue for frames to send over TCP, if needed

    std::thread cameraThread;
    std::thread preprocessingThread;
    std::thread faceDetectionThread;
    std::thread tcpThread;  // Add a thread for the TCP component

    int tcpPort;  // Store the TCP port number
    bool running;

    void cameraLoop();
    void preprocessingLoop();
    void faceDetectionLoop();
    void commtcpLoop();  // Add a loop for the TCP component, if needed

    // Any additional methods for handling TCP communication
};

