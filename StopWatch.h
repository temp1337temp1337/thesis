//
// Created by SC Development Team
// Modified by edaravig on 8/2/2017.
//
#ifndef BKZ_UPDATED_STOPWATCH_H
#define BKZ_UPDATED_STOPWATCH_H
#include <chrono>

class StopWatch {
public:
    StopWatch() : m_start(Clock::now()) {}
    template <class TimeUnit>
    TimeUnit elapsed() const {
        auto now = Clock::now();
        return std::chrono::duration_cast<TimeUnit>(now - m_start);
    }
    void reset() {
        m_start = Clock::now();
    }
private:
    using Clock = std::chrono::high_resolution_clock;
    std::chrono::high_resolution_clock::time_point m_start;
};
#endif //BKZ_UPDATED_STOPWATCH_H