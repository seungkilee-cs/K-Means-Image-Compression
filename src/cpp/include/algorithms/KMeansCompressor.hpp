#pragma once

#include "algorithms/ICompressor.hpp"

#include <opencv2/core.hpp>

namespace imgc::algorithms {

class KMeansCompressor : public ICompressor {
public:
    struct Config {
        int max_iterations = 20;
        double epsilon = 1.0;
        int attempts = 3;
        int flags = cv::KMEANS_PP_CENTERS;
    };

    KMeansCompressor();
    explicit KMeansCompressor(Config config);

    CompressionResult compress(const cv::Mat& samples, int clusters) const override;

private:
    cv::TermCriteria criteria_;
    int attempts_;
    int flags_;
};

}  // namespace imgc::algorithms

