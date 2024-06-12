#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <chrono>

class Performance {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    static TimePoint now() {
        return Clock::now();
    }

    template<typename Duration = std::chrono::milliseconds>
    static auto duration(TimePoint start, TimePoint end) {
        return std::chrono::duration_cast<Duration>(end - start).count();
    }
};

#endif // PERFORMANCE_H
