// PreprocessingComponent.h

#pragma once

#include <opencv2/opencv.hpp>
#include <map>
#include <string>

class IPreprocessingComponent {
public:
    virtual ~IPreprocessingComponent() = default;

    // Process a given frame and return the processed frame
    virtual cv::Mat processFrame(const cv::Mat& inputFrame) = 0;

    // Optionally, configure preprocessing parameters
    virtual void setParameter(const std::string& param, double value) = 0;
};

