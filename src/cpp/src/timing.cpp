#include "timing.hpp"

namespace imgc::timing {

Stopwatch::Stopwatch()
    : start_(clock::now()), last_(start_) {}

long long Stopwatch::tick(const std::string& label) {
    auto now = clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_).count();
    events_.push_back({label, elapsed});
    last_ = now;
    return elapsed;
}

const std::vector<Stopwatch::Event>& Stopwatch::events() const {
    return events_;
}

long long Stopwatch::total_ms() const {
    long long total = 0;
    for (const auto& event : events_) {
        total += event.milliseconds;
    }
    return total;
}

}  // namespace imgc::timing

