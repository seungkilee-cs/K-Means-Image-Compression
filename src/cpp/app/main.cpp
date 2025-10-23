#include "algorithms/KMeansCompressor.hpp"
#include "cli.hpp"
#include "image_io.hpp"
#include "image_preprocessor.hpp"
#include "timing.hpp"

#include <opencv2/core.hpp>

#include <iostream>

int main(int argc, char** argv) {
    const char* program = (argc > 0) ? argv[0] : "kmeans_imgc";

    try {
        auto options = imgc::cli::parse_args(argc, argv);

        std::cout << "[INFO] Input: " << options.input_path << "\n";
        std::cout << "[INFO] k=" << options.clusters
                  << " out_dir=" << options.output_dir
                  << " quality=" << options.jpeg_quality << "\n";

        imgc::timing::Stopwatch stopwatch;

        cv::Mat image = imgc::io::load_color_image(options.input_path);
        std::cout << "[INFO] Loaded " << image.cols << "x" << image.rows << "\n";
        stopwatch.tick("load");

        cv::Mat samples = imgc::preprocess::to_samples(image);
        std::cout << "[INFO] Samples: " << samples.rows << " x " << samples.cols << "\n";
        stopwatch.tick("prep");

        imgc::algorithms::KMeansCompressor compressor;
        std::cout << "[INFO] Running kmeans...\n";
        auto result = compressor.compress(samples, options.clusters);
        std::cout << "[INFO] Kmeans done. Compactness=" << result.compactness << "\n";
        stopwatch.tick("kmeans");

        cv::Mat compressed_image = imgc::preprocess::samples_to_image(
            result.compressed_samples, image.rows, image.cols);
        stopwatch.tick("reconstruct");

        imgc::io::ensure_directory(options.output_dir);
        std::string output_path = imgc::io::make_output_path(
            options.input_path, options.output_dir, options.clusters, options.jpeg_quality);
        imgc::io::save_jpeg(output_path, compressed_image, options.jpeg_quality);
        stopwatch.tick("write");

        std::cout << "[OK] Saved: " << output_path << "\n";

        std::cout << "[TIMING]";
        for (const auto& event : stopwatch.events()) {
            std::cout << ' ' << event.label << '=' << event.milliseconds << "ms";
        }
        std::cout << " total=" << stopwatch.total_ms() << "ms\n";

        return 0;
    } catch (const imgc::cli::UsageError& ex) {
        std::cerr << "[ERROR] " << ex.what() << "\n";
        imgc::cli::print_usage(program);
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << "[ERROR] " << ex.what() << "\n";
        return 2;
    }
}
