#include "facedetectioncomponent.h"

FaceDetectionComponent::FaceDetectionComponent(ThreadSafeQueue<cv::Mat>& inputQueue, ThreadSafeQueue<cv::Mat>& outputQueue)
: inputQueue(inputQueue), outputQueue(outputQueue), running(false) {}

FaceDetectionComponent::~FaceDetectionComponent() {
    stopDetection();
}

bool FaceDetectionComponent::initialize(const std::string& modelConfiguration, const std::string& modelWeights) {
    net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);// Jetson, you might switch to DNN_TARGET_OPENCL or DNN_TARGET_CUDA for better performance
    if (net.empty()) {
        std::cerr << "Failed to load the model or config file." << std::endl;
        return false;
    }
    return true;
}

void FaceDetectionComponent::startDetection() {
    if (running) {
        std::cerr << "Detection is already running." << std::endl;
        return;
    }
    running = true;
    detectionThread = std::thread(&FaceDetectionComponent::detectionLoop, this);
}

void FaceDetectionComponent::stopDetection() {
    running = false;
    if (detectionThread.joinable()) {
        detectionThread.join();
    }
}

void FaceDetectionComponent::detectionLoop() {
    cv::Mat frame;
    while (running) {
        if (inputQueue.tryPop(frame)) {
            detectFaces(frame);
            outputQueue.push(frame);
        }
    }
}

void FaceDetectionComponent::detectFaces(cv::Mat& frame) {
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1/255.0, cv::Size(320, 320), cv::Scalar(0, 0, 0), true, false);//416,416
    net.setInput(blob);

    std::vector<cv::Mat> outs;
    net.forward(outs, net.getUnconnectedOutLayersNames());

    float confThreshold = 0.7;// Confidence threshold
    for (const auto& out : outs) {
        for (int i = 0; i < out.rows; ++i) {
            const float* detection = out.ptr<float>(i);
            float confidence = detection[4];
            if (confidence > confThreshold) {
                int centerX = static_cast<int>(detection[0] * frame.cols);
                int centerY = static_cast<int>(detection[1] * frame.rows);
                int width = static_cast<int>(detection[2] * frame.cols);
                int height = static_cast<int>(detection[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                cv::rectangle(frame, cv::Point(left, top), cv::Point(left + width, top + height), cv::Scalar(0, 255, 0), 3);
            }
        }
    }
}

