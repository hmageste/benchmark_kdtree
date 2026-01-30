# K-D Tree Benchmark - Quick Start Guide

## Overview

This C++ project benchmarks two spatial search algorithms for finding which grid cell a 2D point falls into:
- **Regular Search**: Brute-force checking all 10,000 grid cells
- **K-D Tree Search**: Space-partitioning binary search tree

**Key Result: K-D Tree is ~13x faster** for 100,000 test points!

## Quick Start (3 Steps)

### 1. Build the Project
```bash
cd /home/hmageste/cpp_projects/benchmark_kdtree_algorithm
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

### 2. Run the Benchmark
```bash
# From the build directory
./benchmark_kdtree
```

### 3. View Results
```bash
# Check the generated files
cat benchmark_results.json          # Detailed JSON results
cat benchmark_results.csv           # Spreadsheet-ready CSV

# Generate visualizations (requires Python + matplotlib)
cd ..
python3 visualize_results.py build/benchmark_results.json
```

## Project Structure

```
benchmark_kdtree_algorithm/
├── CMakeLists.txt              # Build configuration
├── README.md                   # Full documentation
├── run_benchmark.sh            # Convenient benchmark runner
├── visualize_results.py        # Chart generation script
├── include/                    # Header files
│   ├── point.h                # Point data structure
│   ├── grid.h                 # Grid and Cell classes
│   ├── kdtree.h               # K-D Tree implementation
│   ├── point_generator.h      # Random point generation
│   └── benchmark_runner.h     # Benchmark infrastructure
├── src/
│   └── main.cpp               # Main benchmark program
├── build/                     # Compiled binaries (generated)
│   └── benchmark_kdtree       # Executable
└── results/                   # Output files
    ├── benchmark_results.json
    ├── benchmark_results.csv
    └── *.png                  # Generated charts
```

### Time Complexity

| Method | Worst Case | Average Case | Space |
|--------|-----------|--------------|-------|
| Regular Search | O(N) | O(N) | O(N) |
| K-D Tree | O(N) | O(log N) | O(N) |

Where N = number of grid cells (10,000 in default config)

## Files Generated

After running, you'll get:

| File | Purpose |
|------|---------|
| benchmark_results.json | Detailed results + metadata (JSON) |
| benchmark_results.csv | Results in spreadsheet format |
| benchmark_total_time.png | Bar chart: total execution time |
| benchmark_avg_time.png | Bar chart: per-iteration time |
| benchmark_memory.png | Bar chart: memory usage |
| benchmark_speedup.png | Speedup factor visualization |
| benchmark_summary.png | Config + results summary |

### Using the Shell Script
```bash
./run_benchmark.sh                 # Run with defaults
./run_benchmark.sh -o ./my_results # Save results to custom directory
./run_benchmark.sh --help          # See all options
```

### Custom Compilation
```bash
# Debug build (includes debug symbols, slower)
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

# With different compiler
cmake -DCMAKE_CXX_COMPILER=clang++ ..
cmake --build .
```

## Troubleshooting

### Build fails with "cmake not found"
```bash
sudo apt-get install cmake build-essential g++
```

### Missing matplotlib for visualizations
```bash
sudo apt-get install python3-matplotlib
```

### Benchmark runs but results are strange
- Ensure Release build: `cmake -DCMAKE_BUILD_TYPE=Release ..`
- Close other programs to reduce system noise
- Try with more test points for clearer timing

## Project Architecture

### Key Components

**Grid System:**
- 100×100 uniform square cells in a 15×15 cm coordinate space
- Each cell has a center point and boundaries
- Points are checked if they fall within cell boundaries

**Regular Search:**
- Iterates through ALL grid cells
- Checks each cell's `contains()` method
- O(N) complexity per search

**K-D Tree:**
- Binary space-partitioning tree
- Recursively divides 2D space along X and Y axes
- O(log N) average search complexity
- Slightly higher memory overhead

**Point Generation:**
- Generates random points uniformly within a circle
- Uses polar coordinate conversion for uniform distribution
- Circle radius: 7.5 cm, centered at (7.5, 7.5)

## Next Steps

1. **Try different grid sizes** (50×50, 200×200, 1000×1000)
2. **Benchmark with different point distributions** (clustered, uniform)
3. **Implement R-Tree or Quadtree** for comparison
4. **Add multi-threaded variants** for parallel searching
5. **Profile with perf or valgrind** for detailed analysis
