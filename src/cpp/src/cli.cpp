#include "cli.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

int parse_integer(const char* value, const char* name) {
    try {
        std::size_t pos = 0;
        int parsed = std::stoi(value, &pos);
        if (value[pos] != '\0') {
            throw std::invalid_argument("contains trailing characters");
        }
        return parsed;
    } catch (const std::exception& ex) {
        std::ostringstream msg;
        msg << "invalid value for " << name << ": '" << value << "' (" << ex.what() << ")";
        throw imgc::cli::UsageError(msg.str());
    }
}

}  // namespace

namespace imgc::cli {

Options parse_args(int argc, char** argv) {
    if (argc < 4) {
        throw UsageError("missing required arguments");
    }

    Options options{};
    options.input_path = argv[1];
    options.clusters = parse_integer(argv[2], "k");
    options.output_dir = argv[3];
    options.jpeg_quality = (argc >= 5) ? parse_integer(argv[4], "jpeg_quality") : 95;

    if (options.clusters < 2 || options.clusters > 256) {
        throw UsageError("k must be in [2, 256]");
    }
    if (options.jpeg_quality < 0 || options.jpeg_quality > 100) {
        throw UsageError("jpeg_quality must be in [0, 100]");
    }

    return options;
}

void print_usage(const char* program_name) {
    std::cout << "Usage:\n"
              << "  " << program_name << " <input.png> <k> <output_dir> [jpeg_quality]\n"
              << "Notes:\n"
              << "  - k: number of colors (clusters), e.g., 8, 16, 32, 64\n"
              << "  - jpeg_quality: 0..100 (default 95)\n";
}

}  // namespace imgc::cli

