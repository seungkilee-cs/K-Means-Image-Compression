#include "image_io.hpp"

#include <opencv2/imgcodecs.hpp>

#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;

namespace imgc::io {

cv::Mat load_color_image(const std::string& path) {
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    if (image.empty()) {
        throw std::runtime_error("failed to read image: " + path);
    }
    return image;
}

void ensure_directory(const std::string& directory) {
    std::error_code ec;
    fs::create_directories(directory, ec);
    if (ec) {
        throw std::runtime_error("failed to create directory: " + directory);
    }
}

std::string make_output_path(const std::string& input_path, const std::string& output_dir, int clusters, int quality) {
    fs::path input(input_path);
    std::ostringstream name;
    name << input.stem().string() << "_k" << clusters << "_q" << quality << ".jpg";
    return (fs::path(output_dir) / name.str()).string();
}

void save_jpeg(const std::string& path, const cv::Mat& image, int quality) {
    std::vector<int> params{cv::IMWRITE_JPEG_QUALITY, quality};
    if (!cv::imwrite(path, image, params)) {
        throw std::runtime_error("failed to write image: " + path);
    }
}

}  // namespace imgc::io

