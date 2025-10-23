#pragma once

#include <opencv2/core.hpp>

namespace imgc::preprocess {

/**
 * Flatten a 3-channel 8-bit image into an N x 3 matrix of 32-bit floats
 * suitable for clustering algorithms such as k-means.
 */
cv::Mat to_samples(const cv::Mat& bgr8u);

/**
 * Convert an N x 3 matrix of 32-bit floats back into a 3-channel 8-bit image.
 * `rows` and `cols` describe the desired output dimensions.
 */
cv::Mat samples_to_image(const cv::Mat& samples32f, int rows, int cols);

}  // namespace imgc::preprocess

