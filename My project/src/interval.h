#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
public:
    interval() : min_(+infinity), max_(-infinity) {} // Default interval is empty

    interval(double min, double max) : min_(min), max_(max) {}

    bool contains(double value) const {
        return min_ <= value && value <= max_;
    }

    bool surrounds(double value) const {
        return min_ < value && value < max_;
    }

    double clamp(double value) const {
        if (value < min_) { return min_; }
        if (value > max_) { return max_; }
        return value;
    }

    double min() const { return min_; }

    double max() const { return max_; }

    static const interval empty, universe;

private:
    double min_, max_;
};

const interval interval::empty(+infinity, -infinity);
const interval interval::universe(-infinity, +infinity);

#endif