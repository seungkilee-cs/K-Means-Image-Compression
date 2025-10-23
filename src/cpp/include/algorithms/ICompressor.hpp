#pragma once

#include <opencv2/core.hpp>

namespace imgc::algorithms {

struct CompressionResult {
    cv::Mat compressed_samples;
    cv::Mat labels;
    cv::Mat centers;
    double compactness;
};

class ICompressor {
public:
    virtual ~ICompressor() = default;
    virtual CompressionResult compress(const cv::Mat& samples, int clusters) const = 0;
};

}  // namespace imgc::algorithms

