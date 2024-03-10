#include <opencv2/opencv.hpp>
#include "threadsafequeue.h"
#include "dmsmanager.h"

int main() {
    // Initialize thread-safe queues
    ThreadSafeQueue<cv::Mat> cameraQueue;
    ThreadSafeQueue<cv::Mat> preprocessingQueue;
    ThreadSafeQueue<cv::Mat> faceDetectionQueue;  // Add a queue for face detection output

    // Initialize the DMSManager with all necessary queues
    DMSManager dmsManager(cameraQueue, preprocessingQueue, faceDetectionQueue);

    // Initialize the camera component
    if (!dmsManager.initializeCamera("/dev/video0")) {  // or use a video file path
        std::cerr << "Failed to initialize camera component." << std::endl;
        return -1;
    }

    // Initialize the face detection component (assuming a method exists in DMSManager)
    // You'll need to provide paths to the YOLOv3-tiny configuration and weights files
    if (!dmsManager.initializeFaceDetection("/home/dms/DMS/ModularCode/modelconfigs/face-yolov3-tiny.cfg", "/home/dms/DMS/ModularCode/modelconfigs/face-yolov3-tiny_41000.weights")) {
        std::cerr << "Failed to initialize face detection component." << std::endl;
        return -1;
    }

    // Start the system
    if (!dmsManager.startSystem()) {
        std::cerr << "Failed to start the system." << std::endl;
        return -1;
    }

    // Main loop
    cv::Mat cameraFrame, preprocessedFrame, faceDetectedFrame;
    while (true) {
        // Display camera frame
        //if (cameraQueue.tryPop(cameraFrame) && !cameraFrame.empty()) {
        //    cv::imshow("Camera Frame", cameraFrame);
        //}

        // Display preprocessed frame
        //if (preprocessingQueue.tryPop(preprocessedFrame) && !preprocessedFrame.empty()) {
        //    cv::imshow("Processed Frame", preprocessedFrame);
        //}

        // Display face detection output
        if (faceDetectionQueue.tryPop(faceDetectedFrame) && !faceDetectedFrame.empty()) {
            cv::imshow("Face Detection", faceDetectedFrame);
        }

        if (cv::waitKey(1) == 27) break;  // Exit on ESC key
    }

    // Shutdown the system
    dmsManager.stopSystem();
    std::cout << "System stopped." << std::endl;

    return 0;
}

