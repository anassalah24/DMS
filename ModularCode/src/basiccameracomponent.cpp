// BasicCameraComponent.cpp

#include "basiccameracomponent.h"

BasicCameraComponent::BasicCameraComponent(ThreadSafeQueue<cv::Mat>& outputQueue)
: outputQueue(outputQueue), running(false) {}

BasicCameraComponent::~BasicCameraComponent() {
    stopCapture();
}

bool BasicCameraComponent::initialize(const std::string& source) {
    if (!cap.open(source)) {
        std::cerr << "Failed to open camera or video file: " << source << std::endl;
        return false;
    }
    return true;
}

void BasicCameraComponent::startCapture() {
    if (running) {
        std::cerr << "Capture is already running." << std::endl;
        return;
    }
    running = true;
    captureThread = std::thread(&BasicCameraComponent::captureLoop, this);
}

void BasicCameraComponent::stopCapture() {
    running = false;
    if (captureThread.joinable()) {
        captureThread.join();
    }
    if (cap.isOpened()) {
        cap.release();
    }
}

void BasicCameraComponent::captureLoop() {
    while (running) {
        cv::Mat frame;
        if (!cap.read(frame)) {
            std::cerr << "Failed to capture frame." << std::endl;
            running = false;
            break;
        }
        if (!frame.empty()) {
            outputQueue.push(frame);
        }
    }
}

