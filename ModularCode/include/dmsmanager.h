#include <thread>
#include "threadsafequeue.h"
#include "basiccameracomponent.h"
#include "basicpreprocessingcomponent.h"
#include "facedetectioncomponent.h"

class DMSManager {
public:
    DMSManager(ThreadSafeQueue<cv::Mat>& cameraQueue, ThreadSafeQueue<cv::Mat>& preprocessingQueue,ThreadSafeQueue<cv::Mat>& faceDetectionQueue );
    ~DMSManager();

    bool startSystem();
    void stopSystem();
    bool initializeCamera(const std::string& source);
    bool initializeFaceDetection(const std::string& modelConfiguration, const std::string& modelWeights);


private:
    BasicCameraComponent cameraComponent;
    BasicPreprocessingComponent preprocessingComponent;
    FaceDetectionComponent faceDetectionComponent;  // Add this line
    ThreadSafeQueue<cv::Mat>& cameraQueue;
    ThreadSafeQueue<cv::Mat>& preprocessingQueue;
    ThreadSafeQueue<cv::Mat>& faceDetectionQueue;  // Add a new queue for face detection output
    std::thread cameraThread;
    std::thread preprocessingThread;
    std::thread faceDetectionThread;  // Add this line
    bool running;

    void cameraLoop();
    void preprocessingLoop();
    void faceDetectionLoop();

};

