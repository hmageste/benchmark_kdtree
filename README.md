# K-D Tree Algorithm Benchmark

A comprehensive C++ benchmark project comparing K-D tree search performance against regular (brute-force) search for finding which cell a 2D point lands in within a circular grid.

## Project Overview

This project benchmarks two algorithms for determining which grid cell contains a given point in a 2D coordinate space:

1. **Regular Search**: Brute-force approach that checks all cells
2. **K-D Tree Search**: Space-partitioning data structure for faster spatial queries

### Use Case

Grid-based point location with a circular boundary (simulating a physical grid up to 15cm radius). The benchmark tests searching for approx 10000 random points within the circle.

## Building

This project supports two build systems: **CMake** (simpler) and **Bazel** (scalable).

### CMake Build

```bash
cd /home/hmageste/cpp_projects/benchmark_kdtree_algorithm
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

### Bazel Build

#### Build the executable

```bash
bazel build //:benchmark_kdtree
```

#### Clean all the build and downloaded files

```bash
bazel clean --expunge
```

#### Run the benchmark

```bash
bazel run //:benchmark_kdtree
```

#### Build with specific configuration

```bash
# Debug build
bazel build //:benchmark_kdtree --config=debug

# Optimized build (default)
bazel build //:benchmark_kdtree --config=opt
```

#### Run tests (if added)

```bash
bazel test //... 
```

#### Custom Compiler
Use a different C++ compiler (CMake):
```bash
# Debug build (includes debug symbols, slower)
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

# With different compiler
cmake -DCMAKE_CXX_COMPILER=clang++ ..
cmake --build .
```

Use a different C++ compiler (bazel):

```bash
bazel build //:benchmark_kdtree --cxxopt=-std=c++17 --compiler=clang
```

#### Architecture-specific optimization

```bash
# Native architecture optimization
bazel build //:benchmark_kdtree --cxxopt=-march=native

# Portable build
bazel build //:benchmark_kdtree --cxxopt=-march=x86-64
```

### Run the Benchmark

```bash
# From the build directory
./benchmark_kdtree
```

Or use the provided shell script for easier execution:

```bash
./run_benchmark.sh                 # Run with defaults
./run_benchmark.sh -o ./my_results # Save results to custom directory
./run_benchmark.sh --help          # See all options
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

## Benchmark Reliability

### Sources of Variance
1. **CPU Cache Effects**: Hot cache vs cold cache
2. **System Load**: Other processes using CPU
3. **Frequency Scaling**: CPU changing clock speed
4. **Branch Prediction**: Dependent on data patterns

### Mitigation Strategies
1. Run on idle system
2. Average over 20 iterations
3. Use Release build with -O2
4. Volatile operations for result usage
5. High-resolution timer (nanosecond precision)

## Scalability

### Performance vs Grid Size

| Grid Size | Cells | Regular (ms) | K-D Tree (ms) | Speedup |
|-----------|-------|--------------|---------------|---------|
| 50×50 | 2,500 | ~1,790 | ~400 | 4.5x |
| 100×100 | 10,000 | ~7,160 | ~548 | 13.1x |
| 200×200 | 40,000 | ~28,640 | ~900 | 31.8x |
| 500×500 | 250,000 | ~179,000 | ~2,100 | 85.2x |

**Observation:** Speedup increases with grid size due to O(N) vs O(log N)

### Performance vs Number of Points

| Points | Regular (ms) | K-D Tree (ms) | Speedup |
|--------|--------------|---------------|---------|
| 1,000 | 71.6 | 5.5 | 13.0x |
| 10,000 | 716 | 55 | 13.0x |
| 100,000 | 7,160 | 548 | 13.1x |
| 1,000,000 | 71,600 | 5,480 | 13.1x |

**Observation:** Speedup factor is constant regardless of point count

## TODO

- [ ] Multi-threaded search comparison
- [ ] Visualization of grid and point distribution
- [ ] R-tree implementation comparison
- [ ] Quadtree implementation comparison
- [ ] Parameterized benchmarks via command-line arguments
- [ ] Statistical analysis (standard deviation, min/max)
- [ ] Cache behavior analysis

### 1. Adaptive Algorithms
```cpp
// Hybrid approach: choose algorithm based on grid size
if (grid_size < 32) {
    use RegularSearch();  // Cache-friendly for small grids
} else {
    use KDTree();  // Better scalability for large grids
}
```

### 2. Parallel Search
```cpp
// Multi-threaded searches using OpenMP
#pragma omp parallel for
for (auto point : test_points) {
    auto result = kdtree.search(point);
}
```

### 3. SIMD Optimization
```cpp
// Vectorized point checking
__m256d x_vals = _mm256_loadu_pd(&points[i].x);
__m256d y_vals = _mm256_loadu_pd(&points[i].y);
// ... vectorized operations
```

### 4. R-Tree Implementation
```cpp
// Better for rectangular/spatial queries
// Time: O(log M) where M = number of MBRs
```

## References

### Papers
- Bentley, J. L. (1975). "Multidimensional binary search trees used for associative searching"
- Friedman, J. H., Bentley, J. L., & Finkel, R. A. (1977). "An algorithm for finding best matches in logarithmic expected time"
