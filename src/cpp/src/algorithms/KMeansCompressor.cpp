#include "algorithms/KMeansCompressor.hpp"

#include <stdexcept>

namespace imgc::algorithms {

namespace {

void validate_samples(const cv::Mat& samples, int clusters) {
    if (samples.empty()) {
        throw std::invalid_argument("samples matrix is empty");
    }
    if (samples.type() != CV_32F) {
        throw std::invalid_argument("samples matrix must be CV_32F");
    }
    if (samples.cols != 3) {
        throw std::invalid_argument("samples matrix must have exactly 3 columns");
    }
    if (clusters < 2 || clusters > 256) {
        throw std::invalid_argument("clusters must be in [2, 256]");
    }
    if (samples.rows < clusters) {
        throw std::invalid_argument("number of samples must be >= clusters");
    }
}

}  // namespace

KMeansCompressor::KMeansCompressor()
    : KMeansCompressor(Config{}) {}

KMeansCompressor::KMeansCompressor(Config config)
    : criteria_(cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER,
                                 config.max_iterations,
                                 config.epsilon)),
      attempts_(config.attempts),
      flags_(config.flags) {}

CompressionResult KMeansCompressor::compress(const cv::Mat& samples, int clusters) const {
    validate_samples(samples, clusters);

    cv::Mat labels;
    cv::Mat centers;
    double compactness = cv::kmeans(samples, clusters, labels, criteria_, attempts_, flags_, centers);

    cv::Mat compressed(samples.size(), samples.type());
    for (int i = 0; i < samples.rows; ++i) {
        int cluster_id = labels.at<int>(i, 0);
        const float* center = centers.ptr<float>(cluster_id);
        float* dst = compressed.ptr<float>(i);
        dst[0] = center[0];
        dst[1] = center[1];
        dst[2] = center[2];
    }

    return CompressionResult{compressed, labels, centers, compactness};
}

}  // namespace imgc::algorithms

