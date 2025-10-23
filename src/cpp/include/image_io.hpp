#pragma once

#include <opencv2/core.hpp>

#include <string>

namespace imgc::io {

cv::Mat load_color_image(const std::string& path);
void ensure_directory(const std::string& directory);
std::string make_output_path(const std::string& input_path, const std::string& output_dir, int clusters, int quality);
void save_jpeg(const std::string& path, const cv::Mat& image, int quality);

}  // namespace imgc::io

