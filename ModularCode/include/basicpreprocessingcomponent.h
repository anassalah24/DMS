// BasicPreprocessingComponent.h

#pragma once

#include <opencv2/opencv.hpp>
#include "threadsafequeue.h"
#include <thread>

class BasicPreprocessingComponent {
public:
    BasicPreprocessingComponent(ThreadSafeQueue<cv::Mat>& inputQueue, ThreadSafeQueue<cv::Mat>& outputQueue);
    ~BasicPreprocessingComponent();

    void startProcessing();
    void stopProcessing();

private:
    ThreadSafeQueue<cv::Mat>& inputQueue;
    ThreadSafeQueue<cv::Mat>& outputQueue;
    std::thread processingThread;
    bool running;

    void processingLoop();
    cv::Mat preprocessFrame(const cv::Mat& frame);
};

