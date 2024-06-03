#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <string>
#include <iomanip>

class Clock {
public:
    std::string clock_measure = "milliseconds";

    void start() {
        log_start_ = std::chrono::steady_clock::now();
    }
    void finish() {
        log_finish_ = std::chrono::steady_clock::now();
    }
    auto result() {
        std::chrono::duration<double, std::ratio<1, 1000>> duration = log_finish_ - log_start_;

        auto result = duration.count();
        if (result > 1000 * 3) {
            result /= 1000;
            clock_measure = "seconds";
            if (result > 60 * 3) {
                result /= 60;
                clock_measure = "minutes";
            }
        }
        return result;
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> log_start_;
    std::chrono::time_point<std::chrono::steady_clock> log_finish_;
};

inline std::ostream &operator<<(std::ostream &out, Clock &clock) {
    return out << std::fixed << std::setprecision(2) << "\nTime working:\n" << clock.result() << " "  << clock.clock_measure << "\n";
}

#endif
