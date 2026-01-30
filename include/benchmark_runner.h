#pragma once

#include "point.h"
#include "grid.h"
#include "kdtree.h"
#include <vector>
#include <chrono>
#include <memory>

struct BenchmarkResult {
    std::string method_name;
    double time_ms;
    long long memory_bytes;
    int iterations;
    double avg_time_ms;
    std::vector<Cell> found_cells;
};

class RegularSearch {
private:
    const std::vector<Cell> cells;

public:
    RegularSearch(const std::vector<Cell>& cells)
        : cells(cells) {}

    Cell search(const Point& target) const {
        Cell result(-1, -1, Point(0, 0), 0);
        // Brute force: check all cells until we find the one containing the point
        for (const auto& cell : cells) {
            if (cell.contains(target)) {
                return cell;
            }
        }
        return result;  // Not found
    }
};

class BenchmarkRunner {
private:
    const std::vector<Cell> lookup_cells;
    int iterations;

public:
    BenchmarkRunner(std::vector<Cell> cells, int iters)
        : lookup_cells(cells), iterations(iters) {}

    BenchmarkResult benchmarkRegularSearch(const std::vector<Point>& test_points) {
        RegularSearch searcher(lookup_cells);
        // Get the memory size - all grid cells
        const long long total_cells = (long long)lookup_cells.size();

        volatile int dummy = 0;  // Use volatile to prevent optimization
        auto start = std::chrono::high_resolution_clock::now();

        for (int iter = 0; iter < iterations; ++iter) {
            for (const auto& point : test_points) {
                auto cell = searcher.search(point);
                // Use the result cell to prevent optimization
                if (cell.grid_x >= 0) dummy++;
            }
        }
    
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed_ms =
            std::chrono::duration<double, std::milli>(end - start).count();

        BenchmarkResult result;
        result.method_name = "Regular Search";
        result.time_ms = elapsed_ms;
        result.memory_bytes = total_cells * sizeof(Cell);
        result.iterations = iterations;
        result.avg_time_ms = elapsed_ms / iterations;

        // Do another run to collect found cells
        std::vector<Cell> found_cells;
        for (const auto& point : test_points) {
            auto cell = searcher.search(point);
            // Use the result cell to prevent optimization
            if (cell.grid_x >= 0) dummy++;
            result.found_cells.push_back(cell);
        }

        return result;
    }

    BenchmarkResult benchmarkKDTree(const std::vector<Point>& test_points) {
        auto kdtree = std::make_unique<KDTree>();

        std::vector<std::pair<Point, Cell>> all_cells;
        for (const auto& cell : lookup_cells) {
            all_cells.emplace_back(cell.center, cell);
        }
    
        kdtree->build(all_cells);

        volatile int dummy = 0;  // Use volatile to prevent optimization
        auto start = std::chrono::high_resolution_clock::now();

        for (int iter = 0; iter < iterations; ++iter) {
            for (const auto& point : test_points) {
                auto cell = kdtree->search(point);
                // Use the result to prevent optimization
                if (cell.grid_x >= 0) dummy++;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        double elapsed_ms =
            std::chrono::duration<double, std::milli>(end - start).count();

        BenchmarkResult result;
        result.method_name = "KD-Tree Search";
        result.time_ms = elapsed_ms;
        result.memory_bytes = all_cells.size() * sizeof(KDNode);
        result.iterations = iterations;
        result.avg_time_ms = elapsed_ms / iterations;

         // Do another run to collect found cells
        std::vector<Cell> found_cells;
        for (const auto& point : test_points) {
            auto cell = kdtree->search(point);
            // Use the result cell to prevent optimization
            if (cell.grid_x >= 0) dummy++;
            result.found_cells.push_back(cell);
        }

        return result;
    }
};
