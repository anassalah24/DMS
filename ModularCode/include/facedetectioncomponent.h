#pragma once

#include <opencv2/opencv.hpp>
#include "threadsafequeue.h"
#include <thread>

class FaceDetectionComponent {
public:
    FaceDetectionComponent(ThreadSafeQueue<cv::Mat>& inputQueue, ThreadSafeQueue<cv::Mat>& outputQueue);
    ~FaceDetectionComponent();

    bool initialize(const std::string& modelConfiguration, const std::string& modelWeights);
    void startDetection();
    void stopDetection();


private:
    ThreadSafeQueue<cv::Mat>& inputQueue;
    ThreadSafeQueue<cv::Mat>& outputQueue;
    cv::dnn::Net net;
    std::thread detectionThread;
    bool running;

    void detectionLoop();
    void detectFaces(cv::Mat& frame);
    int frameCounter = 0;
    int skipRate = 3;
};

