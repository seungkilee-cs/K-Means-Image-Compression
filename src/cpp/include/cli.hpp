#pragma once

#include <stdexcept>
#include <string>

namespace imgc::cli {

struct Options {
    std::string input_path;
    int clusters;
    std::string output_dir;
    int jpeg_quality;
};

class UsageError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

Options parse_args(int argc, char** argv);
void print_usage(const char* program_name);

}  // namespace imgc::cli

