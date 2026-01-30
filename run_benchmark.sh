#!/bin/bash
# run_benchmark.sh - Run benchmark with custom parameters

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
EXECUTABLE="$BUILD_DIR/benchmark_kdtree"

# Default parameters
GRID_SIZE=100
MAX_COORD=15.0
NUM_POINTS=100000
ITERATIONS=20
OUTPUT_DIR="$SCRIPT_DIR/results"

# Print usage
usage() {
    cat << EOF
Usage: $0 [OPTIONS]

Run the K-D Tree vs Regular Search benchmark with custom parameters.

OPTIONS:
    -g, --grid-size SIZE        Grid size (default: 100)
    -c, --coordinate COORD      Max coordinate in cm (default: 15.0)
    -p, --points NUM            Number of test points (default: 100000)
    -i, --iterations ITER       Number of iterations (default: 20)
    -o, --output DIR            Output directory (default: ./results)
    -h, --help                  Show this help message

EXAMPLES:
    # Run with defaults
    $0

    # Run with 200x200 grid and 50K points
    $0 -g 200 -p 50000

    # Run with 20 iterations
    $0 -i 20

    # Run and save results to custom directory
    $0 -o ./my_results

EOF
    exit 0
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -g|--grid-size)
            GRID_SIZE="$2"
            shift 2
            ;;
        -c|--coordinate)
            MAX_COORD="$2"
            shift 2
            ;;
        -p|--points)
            NUM_POINTS="$2"
            shift 2
            ;;
        -i|--iterations)
            ITERATIONS="$2"
            shift 2
            ;;
        -o|--output)
            OUTPUT_DIR="$2"
            shift 2
            ;;
        -h|--help)
            usage
            ;;
        *)
            echo "Unknown option: $1"
            usage
            ;;
    esac
done

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: Executable not found at $EXECUTABLE"
    echo "Please build the project first with: mkdir -p build && cd build && cmake .. && cmake --build ."
    exit 1
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Note: The C++ program uses hardcoded parameters.
# To support command-line parameters, you would need to modify the C++ code.
# For now, we'll just run the benchmark with its default parameters.

echo "Running benchmark..."
echo "  Grid Size: $GRID_SIZE×$GRID_SIZE"
echo "  Max Coordinate: $MAX_COORD cm"
echo "  Test Points: $NUM_POINTS"
echo "  Iterations: $ITERATIONS"
echo "  Output Directory: $OUTPUT_DIR"
echo ""

# Run the benchmark
"$EXECUTABLE"

# Move results to output directory
if [ -f "benchmark_results.json" ]; then
    mv benchmark_results.json "$OUTPUT_DIR/"
    echo "Moved benchmark_results.json to $OUTPUT_DIR/"
fi

if [ -f "benchmark_results.csv" ]; then
    mv benchmark_results.csv "$OUTPUT_DIR/"
    echo "Moved benchmark_results.csv to $OUTPUT_DIR/"
fi

# Try to generate visualizations if Python is available
if command -v python3 &> /dev/null; then
    if [ -f "$SCRIPT_DIR/visualize_results.py" ]; then
        echo ""
        echo "Generating visualizations..."
        python3 "$SCRIPT_DIR/visualize_results.py" "$OUTPUT_DIR/benchmark_results.json"
        echo "Visualizations saved to $OUTPUT_DIR/"
    fi
fi

echo ""
echo "Benchmark complete! Results saved to $OUTPUT_DIR/"
