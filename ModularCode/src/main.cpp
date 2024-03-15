#include <opencv2/opencv.hpp>
#include "threadsafequeue.h"
#include "dmsmanager.h"

int main() {
    // Initialize thread-safe queues
    ThreadSafeQueue<cv::Mat> cameraQueue;
    ThreadSafeQueue<cv::Mat> preprocessingQueue;
    ThreadSafeQueue<cv::Mat> faceDetectionQueue;  // Queue for face detection output
    ThreadSafeQueue<cv::Mat> tcpOutputQueue;      // Queue for TCP output, if needed

    int tcpPort = 12345;  // Define the TCP port for the server

    // Initialize the DMSManager with all necessary queues and the TCP port
    DMSManager dmsManager(cameraQueue, preprocessingQueue, faceDetectionQueue, tcpOutputQueue, tcpPort);

    // Initialize the camera component
    if (!dmsManager.initializeCamera("/dev/video0")) {  // or use a video file path
        std::cerr << "Failed to initialize camera component." << std::endl;
        return -1;
    }

    // Initialize the face detection component
    if (!dmsManager.initializeFaceDetection("/home/anas/DMS-main/ModularCode/modelconfigs/face-yolov3-tiny.cfg", "/home/anas/DMS-main/ModularCode/modelconfigs/face-yolov3-tiny_41000.weights")) {
        std::cerr << "Failed to initialize face detection component." << std::endl;
        return -1;
    }

    // Start the system
    if (!dmsManager.startSystem()) {
        std::cerr << "Failed to start the system." << std::endl;
        return -1;
    }

    // Main loop (The display code is commented out; uncomment if needed)
    cv::Mat cameraFrame, preprocessedFrame, faceDetectedFrame;
    while (true) {
        //if (cameraQueue.tryPop(cameraFrame) && !cameraFrame.empty()) {
        //    cv::imshow("Camera Frame", cameraFrame);
        //}

        //if (preprocessingQueue.tryPop(preprocessedFrame) && !preprocessedFrame.empty()) {
        //    cv::imshow("Processed Frame", preprocessedFrame);
        //}

        //if (faceDetectionQueue.tryPop(faceDetectedFrame) && !faceDetectedFrame.empty()) {
        //    cv::imshow("Face Detection", faceDetectedFrame);
        //}

        //if (cv::waitKey(1) == 27) break;  // Exit on ESC key
    }

    // Shutdown the system
    dmsManager.stopSystem();
    std::cout << "System stopped." << std::endl;

    return 0;
}

