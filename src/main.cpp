#include "point.h"
#include "grid.h"
#include "kdtree.h"
#include "point_generator.h"
#include "benchmark_runner.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void printResults(const std::vector<BenchmarkResult>& results) {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "BENCHMARK RESULTS" << std::endl;
    std::cout << std::string(70, '=') << std::endl;

    for (const auto& result : results) {
        std::cout << "\nMethod: " << result.method_name << std::endl;
        std::cout << "  Total Time: " << std::fixed << std::setprecision(2)
                  << result.time_ms << " ms" << std::endl;
        std::cout << "  Average Time per Iteration: " << std::fixed
                  << std::setprecision(3) << result.avg_time_ms << " ms" << std::endl;
        std::cout << "  Memory Used: " << std::fixed << std::setprecision(2)
                  << (result.memory_bytes / 1024.0) << " KB" << std::endl;
        std::cout << "  Total Points Searched: " << (result.iterations * 1000)
                  << std::endl;
    }

    if (results.size() == 2) {
        double speedup = results[0].time_ms / results[1].time_ms;
        std::cout << "\n" << std::string(70, '-') << std::endl;
        std::cout << "SPEEDUP (Regular vs KD-Tree): " << std::fixed
                  << std::setprecision(2) << speedup << "x" << std::endl;
        std::cout << std::string(70, '=') << std::endl;
    }
}

json resultsToJson(int grid_size, double min_coord, double max_coord, int num_points, int iterations,
                   const std::vector<BenchmarkResult>& results) {
    json j;
    j["configuration"] = {
        {"grid_size", grid_size},
        {"min_coordinate_cm", min_coord},
        {"max_coordinate_cm", max_coord},
        {"num_test_points", num_points},
        {"iterations", iterations}
    };

    json results_array = json::array();
    for (const auto& result : results) {
        results_array.push_back({
            {"method", result.method_name},
            {"total_time_ms", result.time_ms},
            {"avg_time_per_iteration_ms", result.avg_time_ms},
            {"memory_kb", result.memory_bytes / 1024.0},
            {"total_points_searched", result.iterations * 1000}
        });
    }

    j["results"] = results_array;

    if (results.size() == 2) {
        j["analysis"] = {
            {"speedup_factor", results[0].time_ms / results[1].time_ms},
            {"faster_method", results[1].time_ms < results[0].time_ms ? "KD-Tree" : "Regular"}
        };
    }

    return j;
}

void saveResultsToFile(const std::string& filename, const json& data) {
    std::ofstream file(filename);
    file << data.dump(4) << std::endl;
    file.close();
    std::cout << "\nResults saved to: " << filename << std::endl;
}

void saveResultsToCSV(const std::string& filename,
                      const std::vector<BenchmarkResult>& results) {
    std::ofstream file(filename);
    file << "Method,Total_Time_ms,Avg_Time_Per_Iteration_ms,Memory_KB,Points_Searched\n";

    for (const auto& result : results) {
        file << result.method_name << ","
             << std::fixed << std::setprecision(2) << result.time_ms << ","
             << std::setprecision(3) << result.avg_time_ms << ","
             << std::setprecision(2) << (result.memory_bytes / 1024.0) << ","
             << (result.iterations * 1000) << "\n";
    }

    file.close();
    std::cout << "CSV saved to: " << filename << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    double radius = 15.0;  // 15 cm
    int grid_size = 1000;
    int iterations = 20;  // Increased iterations

    std::cout << "=== K-D Tree vs Regular Search Benchmark ===" << std::endl;
    std::cout << "\nConfiguration:" << std::endl;
    std::cout << "  Circle Radius: " << radius << " cm" << std::endl;
    std::cout << "  Coordinate System: [" << -radius << ", " << radius << "] cm x [" << -radius << ", "
    << radius << "] cm" << std::endl;
    std::cout << "  Grid Size: " << grid_size << " x " << grid_size << std::endl;
    const double step = (2.0 * radius) / grid_size;
    std::cout << "  Grid Size (cm): " << step << "cm x " << step << "cm" << std::endl;
    std::cout << "  Max Test Points: " << static_cast<int>(grid_size * grid_size) << std::endl;
    std::cout << "  Iterations: " << iterations << std::endl;

    // // Generate test points within circle
    // CirclePointGenerator gen(max_coord, 42);
    // auto test_points = gen.generateRegularPointsParallelToXYAxis(num_test_points);

    // // Create grid
    // Grid grid(grid_size, max_coord);

    // // Create cells for all points
    // std::vector<std::pair<Point, Cell>> point_cells;
    // for (const auto& point : test_points) {
    //     point_cells.emplace_back(point, grid.getCellAtPoint(point));
    // }

    std::cout << "\nGenerating regular points in circle with respective cells..." << std::endl;
    std::vector<Point> cell_centers;
    std::vector<Cell> cells;

    const Point center(0.0, 0.0);

    int cell_y_idx = 0;
    int cell_x_idx = 0;
    for (double y = -radius; y <= radius; y += step) {
        for (double x = -radius; x <= radius; x += step) {
            Point p(x, y);
            if (p.distance(center) <= radius) {
                cell_centers.push_back(p);
                cells.emplace_back(cell_x_idx, cell_y_idx, p, step / 2.0);
                cell_x_idx++;
            }
        }
        cell_y_idx++;
        cell_x_idx = 0;
    }

    std::cout << "Generated " << cell_centers.size() << " center points" << std::endl;

    // Random shuffle points for unbiased access patterns
    std::cout << "\nRandom shuffle points for unbiased access patterns" << std::endl;
    std::shuffle(cell_centers.begin(), cell_centers.end(), std::mt19937{std::random_device{}()});

    // Create benchmark runner
    BenchmarkRunner runner(cells, iterations);

    // Run benchmarks
    std::cout << "\nRunning benchmarks..." << std::endl;
    std::vector<BenchmarkResult> results;

    const BenchmarkResult regular_result = runner.benchmarkRegularSearch(cell_centers);
    results.push_back(regular_result);
    std::cout << "  Regular Search: Done" << std::endl;

    const BenchmarkResult kd_result = runner.benchmarkKDTree(cell_centers);
    results.push_back(kd_result);
    std::cout << "  K-D Tree Search: Done" << std::endl;

    if (regular_result.found_cells != kd_result.found_cells) {
        std::cerr << "Error: Found cells do not match between Regular Search and KD-Tree Search." << std::endl;
        return 1;
    }

    // Print results
    printResults(results);

    // Save results
    json json_results =
        resultsToJson(grid_size, -radius, radius, static_cast<int>(cell_centers.size()), iterations, results);
    saveResultsToFile("benchmark_results.json", json_results);
    saveResultsToCSV("benchmark_results.csv", results);

    std::cout << "\nBenchmark complete!" << std::endl;

    return 0;
}
