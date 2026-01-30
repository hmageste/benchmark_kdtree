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

### CMake Build (Recommended for simplicity)

```bash
cd /home/hmageste/cpp_projects/benchmark_kdtree_algorithm
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

### Bazel Build (Recommended for large projects)

#### Build the executable

```bash
bazel build //:benchmark_kdtree
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

Use a different C++ compiler:

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
# From project root
./run_benchmark.sh

# With custom output directory
./run_benchmark.sh -o ./my_results

# View usage
./run_benchmark.sh --help
```

## Future Enhancements

- [ ] Multi-threaded search comparison
- [ ] Visualization of grid and point distribution
- [ ] R-tree implementation comparison
- [ ] Quadtree implementation comparison
- [ ] Parameterized benchmarks via command-line arguments
- [ ] Statistical analysis (standard deviation, min/max)
- [ ] Cache behavior analysis
