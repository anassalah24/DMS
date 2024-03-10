// BasicPreprocessingComponent.cpp

#include "basicpreprocessingcomponent.h"

BasicPreprocessingComponent::BasicPreprocessingComponent(ThreadSafeQueue<cv::Mat>& inputQueue, ThreadSafeQueue<cv::Mat>& outputQueue)
: inputQueue(inputQueue), outputQueue(outputQueue), running(false) {}

BasicPreprocessingComponent::~BasicPreprocessingComponent() {
    stopProcessing();
}

void BasicPreprocessingComponent::startProcessing() {
    if (running) {
        std::cerr << "Processing is already running." << std::endl;
        return;
    }
    running = true;
    processingThread = std::thread(&BasicPreprocessingComponent::processingLoop, this);
}

void BasicPreprocessingComponent::stopProcessing() {
    running = false;
    if (processingThread.joinable()) {
        processingThread.join();
    }
}

void BasicPreprocessingComponent::processingLoop() {
    cv::Mat frame;
    while (running) {
        if (inputQueue.tryPop(frame)) {
            cv::Mat processedFrame = preprocessFrame(frame);
            if (!processedFrame.empty()) {
                outputQueue.push(processedFrame);
            }
        }
    }
}

cv::Mat BasicPreprocessingComponent::preprocessFrame(const cv::Mat& frame) {
    cv::Mat processedFrame;
    // Example preprocessing step: convert to grayscale
    cv::cvtColor(frame, processedFrame, cv::COLOR_BGR2GRAY);
    // Add more preprocessing steps as needed

    return processedFrame;
}

