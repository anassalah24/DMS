#include <opencv2/opencv.hpp>
#include "threadsafequeue.h"
#include "dmsmanager.h"

int main() {
    // Initialize thread-safe queues
    ThreadSafeQueue<cv::Mat> cameraQueue;
    ThreadSafeQueue<cv::Mat> preprocessingQueue;
    DMSManager dmsManager(cameraQueue, preprocessingQueue);

    if (!dmsManager.initializeCamera("/dev/video0")) { // or use a video file path
        std::cerr << "Failed to initialize camera component." << std::endl;
        return -1;
    }

    // Initialize and start the DMSManager

    if (!dmsManager.startSystem()) {
        std::cerr << "Failed to start the system." << std::endl;
        return -1;
    }

    // Main loop
    cv::Mat frame;
    cv::Mat framecam;
    while (true) {
        if (cameraQueue.tryPop(framecam)) {
            if (!framecam.empty()) {
                cv::imshow("camera Frame", framecam);
                if (cv::waitKey(1) == 27) break;  // Exit on ESC key
            }
        }
        if (preprocessingQueue.tryPop(frame)) {
            if (!frame.empty()) {
                cv::imshow("Processed Frame", frame);
                if (cv::waitKey(1) == 27) break;  // Exit on ESC key
            }
        }
    }

    // Shutdown
    dmsManager.stopSystem();
    std::cout << "System stopped." << std::endl;

    return 0;
}

