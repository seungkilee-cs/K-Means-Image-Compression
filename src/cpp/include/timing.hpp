#pragma once

#include <chrono>
#include <string>
#include <vector>

namespace imgc::timing {

class Stopwatch {
public:
    using clock = std::chrono::high_resolution_clock;

    struct Event {
        std::string label;
        long long milliseconds;
    };

    Stopwatch();

    long long tick(const std::string& label);
    const std::vector<Event>& events() const;
    long long total_ms() const;

private:
    clock::time_point start_;
    clock::time_point last_;
    std::vector<Event> events_;
};

}  // namespace imgc::timing

