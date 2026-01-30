#pragma once

#include <cmath>
#include <iostream>

struct Point {
    double x;
    double y;

    Point() : x(0.0), y(0.0) {}
    Point(double x, double y) : x(x), y(y) {}

    double distance(const Point& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    bool operator==(const Point& other) const {
        const double eps = 1e-9;
        return std::abs(x - other.x) < eps && std::abs(y - other.y) < eps;
    }

    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};
