#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>

class Clock {
public:
    void start() {
        log_start_ = std::chrono::steady_clock::now();
    }
    void finish() {
        log_finish_ = std::chrono::steady_clock::now();
    }
    auto result() const {
        std::chrono::duration<double, std::ratio<1, 1000>> duration = log_finish_ - log_start_;
        return duration.count();
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> log_start_;
    std::chrono::time_point<std::chrono::steady_clock> log_finish_;
};

inline std::ostream &operator<<(std::ostream &out, const Clock &clock) {
    return out << std::fixed << std::setprecision(2) << "\nTime working:\n" << clock.result() << " milliseconds\n";
}

#endif
