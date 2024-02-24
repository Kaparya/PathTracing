#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
public:
    interval() : min_(-INF), max_(INF) {}

    interval(float min, float max) : min_(min), max_(max) {}

    bool contains(float value) const {
        return min_ <= value && value <= max_;
    }

    bool surrounds(float value) const {
        return min_ < value && value < max_;
    }

    float clamp(float value) const {
        if (value < min_) { return min_; }
        if (value > max_) { return max_; }
        return value;
    }

    float min() const { return min_; }

    float max() const { return max_; }

    static const interval empty, universe;

private:
    float min_, max_;
};

const interval interval::empty(INF, -INF);
const interval interval::universe(-INF, INF);

#endif
