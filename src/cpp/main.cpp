#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

namespace fs = std::filesystem;

static void usage(const char* prog) {
    std::cout << "Usage:\n"
              << "  " << prog << " <input.png> <k> <output_dir> [jpeg_quality]\n"
              << "Notes:\n"
              << "  - k: number of colors (clusters), e.g., 8, 16, 32, 64\n"
              << "  - jpeg_quality: 0..100 (default 95)\n";
}

static std::string basename_no_ext(const std::string& path) {
    fs::path p(path);
    return p.stem().string();
}

int main(int argc, char** argv) {
    using clock = std::chrono::high_resolution_clock;
    auto t0 = clock::now();

    if (argc < 4) {
        usage(argv[0]);
        return 1;
    }

    std::string in_path = argv[1];
    int k = std::stoi(argv[2]);
    std::string out_dir = argv[3];
    int quality = (argc >= 5) ? std::stoi(argv[4]) : 95;

    std::cout << "[INFO] Input: " << in_path << "\n";
    std::cout << "[INFO] k=" << k << " out_dir=" << out_dir << " quality=" << quality << "\n";

    if (k < 2 || k > 256) {
        std::cerr << "[ERROR] k must be in [2, 256]\n";
        return 2;
    }
    if (quality < 0 || quality > 100) {
        std::cerr << "[ERROR] jpeg_quality must be in [0, 100]\n";
        return 3;
    }

    std::error_code ec;
    fs::create_directories(out_dir, ec);
    if (ec) {
        std::cerr << "[ERROR] cannot create output directory: " << out_dir << "\n";
        return 4;
    }

    // 1) Read image
    auto t1 = clock::now();
    cv::Mat bgr = cv::imread(in_path, cv::IMREAD_COLOR);
    if (bgr.empty()) {
        std::cerr << "[ERROR] failed to read image: " << in_path << "\n";
        return 5;
    }
    std::cout << "[INFO] Loaded " << bgr.cols << "x" << bgr.rows << "\n";

    // 2) Prepare samples (N x 3, CV_32F)
    auto t2 = clock::now();
    cv::Mat bgr_f32;
    bgr.convertTo(bgr_f32, CV_32F);
    if (!bgr_f32.isContinuous()) bgr_f32 = bgr_f32.clone();
    cv::Mat samples = bgr_f32.reshape(1, bgr.rows * bgr.cols);
    std::cout << "[INFO] Samples: " << samples.rows << " x " << samples.cols << "\n";

    // 3) KMeans
    auto t3 = clock::now();
    cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 20, 1.0);
    int attempts = 3;
    int flags = cv::KMEANS_PP_CENTERS;
    cv::Mat labels;   // CV_32S, N x 1
    cv::Mat centers;  // CV_32F, k x 3

    std::cout << "[INFO] Running kmeans...\n";
    double compactness = cv::kmeans(samples, k, labels, criteria, attempts, flags, centers);
    std::cout << "[INFO] Kmeans done. Compactness=" << compactness << "\n";

    // 4) Reconstruct image
    auto t4 = clock::now();
    cv::Mat compressed_flat(samples.size(), samples.type()); // N x 3, CV_32F
    for (int i = 0; i < samples.rows; ++i) {
        int cid = labels.at<int>(i, 0);
        const cv::Vec3f& c = centers.at<cv::Vec3f>(cid, 0);
        compressed_flat.at<float>(i, 0) = c[0];
        compressed_flat.at<float>(i, 1) = c[1];
        compressed_flat.at<float>(i, 2) = c[2];
    }

    // 5) Convert back to uint8 and reshape
    auto t5 = clock::now();
    cv::Mat compressed_u8;
    compressed_flat.convertTo(compressed_u8, CV_8U);
    cv::Mat compressed_bgr = compressed_u8.reshape(3, bgr.rows);

    // 6) Write JPEG
    auto t6 = clock::now();
    std::string base = basename_no_ext(in_path);
    std::string out_path = (fs::path(out_dir) / (base + "_k" + std::to_string(k) + "_q" + std::to_string(quality) + ".jpg")).string();
    std::vector<int> params = { cv::IMWRITE_JPEG_QUALITY, quality };
    if (!cv::imwrite(out_path, compressed_bgr, params)) {
        std::cerr << "[ERROR] failed to write image: " << out_path << "\n";
        return 6;
    }
    auto t7 = clock::now();

    auto ms = [](auto a, auto b){ return std::chrono::duration_cast<std::chrono::milliseconds>(b-a).count(); };
    std::cout << "[OK] Saved: " << out_path << "\n";
    std::cout << "[TIMING] load=" << ms(t1,t2) << "ms prep=" << ms(t2,t3) << "ms "
              << "kmeans=" << ms(t3,t4) << "ms reconstruct=" << ms(t4,t5) << "ms "
              << "reshape=" << ms(t5,t6) << "ms write=" << ms(t6,t7) << "ms "
              << "total=" << ms(t0,t7) << "ms\n";
    return 0;
}
