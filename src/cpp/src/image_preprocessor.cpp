#include "image_preprocessor.hpp"

#include <stdexcept>

namespace imgc::preprocess {

namespace {

void ensure_three_channel_8u(const cv::Mat& mat) {
    if (mat.empty()) {
        throw std::invalid_argument("input image is empty");
    }
    if (mat.type() != CV_8UC3) {
        throw std::invalid_argument("input image must be CV_8UC3");
    }
}

void ensure_samples_32f(const cv::Mat& samples, int expected_rows) {
    if (samples.empty()) {
        throw std::invalid_argument("samples matrix is empty");
    }
    if (samples.type() != CV_32F) {
        throw std::invalid_argument("samples matrix must be CV_32F");
    }
    if (samples.cols != 3) {
        throw std::invalid_argument("samples matrix must have exactly 3 columns");
    }
    if (expected_rows >= 0 && samples.rows != expected_rows) {
        throw std::invalid_argument("samples matrix row count does not match image dimensions");
    }
}

}  // namespace

cv::Mat to_samples(const cv::Mat& bgr8u) {
    ensure_three_channel_8u(bgr8u);

    cv::Mat bgr32f;
    bgr8u.convertTo(bgr32f, CV_32F);
    if (!bgr32f.isContinuous()) {
        bgr32f = bgr32f.clone();
    }
    return bgr32f.reshape(1, bgr8u.rows * bgr8u.cols);
}

cv::Mat samples_to_image(const cv::Mat& samples32f, int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("rows and cols must be positive");
    }

    ensure_samples_32f(samples32f, rows * cols);

    cv::Mat reshaped = samples32f.reshape(3, rows);
    cv::Mat bgr8u;
    reshaped.convertTo(bgr8u, CV_8U);
    return bgr8u;
}

}  // namespace imgc::preprocess

