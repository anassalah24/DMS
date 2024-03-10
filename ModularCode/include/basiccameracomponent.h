// BasicCameraComponent.h

#pragma once

#include <opencv2/opencv.hpp>
#include "threadsafequeue.h"
#include <thread>

class BasicCameraComponent {
public:
    BasicCameraComponent(ThreadSafeQueue<cv::Mat>& outputQueue);
    ~BasicCameraComponent();

    bool initialize(const std::string& source);
    void startCapture();
    void stopCapture();

private:
    cv::VideoCapture cap;
    std::thread captureThread;
    bool running;
    ThreadSafeQueue<cv::Mat>& outputQueue;

    void captureLoop();
};

