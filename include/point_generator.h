#pragma once

#include "point.h"
#include <vector>
#include <random>
#include <cmath>

class CirclePointGenerator {
private:
    std::mt19937 gen;
    double radius;

public:
    CirclePointGenerator(double radius, unsigned seed = 42)
        : gen(seed), radius(radius) {}

    std::vector<Point> generateRandomPointsInCircle(int count) {
        std::cout << "\nGenerating random points in circle..." << std::endl;
        std::vector<Point> points;
        points.reserve(count);

        // Using rejection sampling for uniform distribution in circle
        std::uniform_real_distribution<> dis(0.0, radius);

        int generated = 0;
        while (generated < count) {
            double x = dis(gen);
            double y = dis(gen);
            Point p(x, y);

            // Center of circle at (radius, radius)
            Point center(radius, radius);
            if (p.distance(center) <= radius) {
                points.push_back(p);
                generated++;
            }
        }

        std::cout << "Generated " << points.size() << " points" << std::endl;

        return points;
    }

    std::vector<Point> generateRandomPointsInCircleImproved(int count) {
        std::cout << "\nGenerating random points in circle (improved)..." << std::endl;
        std::vector<Point> points;
        points.reserve(count);

        // Using polar coordinates for uniform distribution in circle
        std::uniform_real_distribution<> r_dis(0.0, radius);
        std::uniform_real_distribution<> theta_dis(0.0, 2.0 * M_PI);

        Point center(radius, radius);

        for (int i = 0; i < count; ++i) {
            // Use sqrt for uniform distribution in circle
            double r = std::sqrt(r_dis(gen) * radius);
            double theta = theta_dis(gen);

            double x = center.x + r * std::cos(theta);
            double y = center.y + r * std::sin(theta);

            // Clamp to grid bounds
            x = std::min(std::max(x, 0.0), radius);
            y = std::min(std::max(y, 0.0), radius);

            points.emplace_back(x, y);
        }

        std::cout << "Generated " << points.size() << " points" << std::endl;

        return points;
    }

    std::vector<Point> generateRegularPointsParallelToXYAxis(int count) {
        std::cout << "\nGenerating regular points in circle..." << std::endl;
        std::vector<Point> points;
        points.reserve(count);

        const double step = (2.0 * radius) / std::sqrt(count);
        const Point center(0.0, 0.0);

        for (double y = -radius; y <= radius; y += step) {
            for (double x = -radius; x <= radius; x += step) {
                Point p(x, y);
                if (p.distance(center) <= radius) {
                    points.push_back(p);
                }
            }
        }

        std::cout << "Generated " << points.size() << " points" << std::endl;

        return points;
    }
};
