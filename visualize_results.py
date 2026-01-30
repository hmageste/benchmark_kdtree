#!/usr/bin/env python3
"""
Visualization script for K-D Tree vs Regular Search benchmark results.
Generates charts from benchmark_results.json
"""

import json
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from pathlib import Path

def load_results(filepath):
    """Load benchmark results from JSON file."""
    with open(filepath, 'r') as f:
        return json.load(f)

def create_visualizations(results, output_dir='.'):
    """Create and save visualization charts."""
    output_dir = Path(output_dir)
    output_dir.mkdir(exist_ok=True)

    config = results['configuration']
    data = results['results']
    analysis = results.get('analysis', {})

    # Extract data
    methods = [d['method'] for d in data]
    times = [d['total_time_ms'] for d in data]
    avg_times = [d['avg_time_per_iteration_ms'] for d in data]
    memory = [d['memory_kb'] for d in data]

    # Color scheme
    colors = ['#FF6B6B', '#4ECDC4']

    # Figure 1: Total Execution Time Comparison
    fig, ax = plt.subplots(figsize=(10, 6))
    bars = ax.bar(methods, times, color=colors, alpha=0.8, edgecolor='black', linewidth=1.5)
    ax.set_ylabel('Time (ms)', fontsize=12, fontweight='bold')
    ax.set_title('Total Execution Time Comparison\n(100K points, 20 iterations)', 
                 fontsize=14, fontweight='bold')
    ax.grid(axis='y', alpha=0.3, linestyle='--')

    # Add value labels on bars
    for bar, time in zip(bars, times):
        height = bar.get_height()
        ax.text(bar.get_x() + bar.get_width()/2., height,
                f'{time:.2f} ms',
                ha='center', va='bottom', fontweight='bold', fontsize=11)

    plt.tight_layout()
    plt.savefig(output_dir / 'benchmark_total_time.png', dpi=300, bbox_inches='tight')
    print(f"Saved: {output_dir / 'benchmark_total_time.png'}")
    plt.close()

    # Figure 2: Average Time Per Iteration
    fig, ax = plt.subplots(figsize=(10, 6))
    bars = ax.bar(methods, avg_times, color=colors, alpha=0.8, edgecolor='black', linewidth=1.5)
    ax.set_ylabel('Time (ms)', fontsize=12, fontweight='bold')
    ax.set_title('Average Time Per Iteration\n(100K points, 20 iterations)', 
                 fontsize=14, fontweight='bold')
    ax.grid(axis='y', alpha=0.3, linestyle='--')

    # Add value labels on bars
    for bar, time in zip(bars, avg_times):
        height = bar.get_height()
        ax.text(bar.get_x() + bar.get_width()/2., height,
                f'{time:.3f} ms',
                ha='center', va='bottom', fontweight='bold', fontsize=11)

    plt.tight_layout()
    plt.savefig(output_dir / 'benchmark_avg_time.png', dpi=300, bbox_inches='tight')
    print(f"Saved: {output_dir / 'benchmark_avg_time.png'}")
    plt.close()

    # Figure 3: Memory Usage Comparison
    fig, ax = plt.subplots(figsize=(10, 6))
    bars = ax.bar(methods, memory, color=colors, alpha=0.8, edgecolor='black', linewidth=1.5)
    ax.set_ylabel('Memory (KB)', fontsize=12, fontweight='bold')
    ax.set_title('Memory Usage Comparison\n(Grid cells storage)', 
                 fontsize=14, fontweight='bold')
    ax.grid(axis='y', alpha=0.3, linestyle='--')

    # Add value labels on bars
    for bar, mem in zip(bars, memory):
        height = bar.get_height()
        ax.text(bar.get_x() + bar.get_width()/2., height,
                f'{mem:.2f} KB',
                ha='center', va='bottom', fontweight='bold', fontsize=11)

    plt.tight_layout()
    plt.savefig(output_dir / 'benchmark_memory.png', dpi=300, bbox_inches='tight')
    print(f"Saved: {output_dir / 'benchmark_memory.png'}")
    plt.close()

    # Figure 4: Performance Comparison (Speedup)
    if len(data) == 2:
        speedup = analysis.get('speedup_factor', 0)
        fig, ax = plt.subplots(figsize=(10, 6))
        
        # Create a simple speedup visualization
        x_pos = [0]
        speedup_val = [speedup]
        bars = ax.barh(x_pos, speedup_val, height=0.5, color='#95E1D3', 
                       alpha=0.8, edgecolor='black', linewidth=2)
        
        ax.set_yticks(x_pos)
        ax.set_yticklabels(['K-D Tree Speedup'])
        ax.set_xlabel('Speedup Factor (vs Regular Search)', fontsize=12, fontweight='bold')
        ax.set_title('K-D Tree Performance Advantage\n(Higher is better)', 
                     fontsize=14, fontweight='bold')
        ax.grid(axis='x', alpha=0.3, linestyle='--')
        ax.set_xlim(0, max(speedup * 1.2, 15))

        # Add value label
        ax.text(speedup + 0.3, 0, f'{speedup:.2f}x faster',
                va='center', fontweight='bold', fontsize=12,
                bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))

        plt.tight_layout()
        plt.savefig(output_dir / 'benchmark_speedup.png', dpi=300, bbox_inches='tight')
        print(f"Saved: {output_dir / 'benchmark_speedup.png'}")
        plt.close()

    # Figure 5: Configuration Summary
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.axis('off')

    summary_text = f"""
    BENCHMARK CONFIGURATION
    
    Grid Size: {config['grid_size']}×{config['grid_size']} cells
    Coordinate Range: [0, {config['max_coordinate_cm']}] cm
    Test Points: {config['num_test_points']:,}
    Iterations: {config['iterations']}
    
    RESULTS SUMMARY
    
    Regular Search:
      • Total Time: {data[0]['total_time_ms']:.2f} ms
      • Per Iteration: {data[0]['avg_time_per_iteration_ms']:.3f} ms
      • Memory: {data[0]['memory_kb']:.2f} KB
    
    K-D Tree Search:
      • Total Time: {data[1]['total_time_ms']:.2f} ms
      • Per Iteration: {data[1]['avg_time_per_iteration_ms']:.3f} ms
      • Memory: {data[1]['memory_kb']:.2f} KB
    
    Speedup: {analysis.get('speedup_factor', 0):.2f}x
    Faster Method: {analysis.get('faster_method', 'N/A')}
    """

    ax.text(0.1, 0.9, summary_text, transform=ax.transAxes,
            fontsize=11, verticalalignment='top', family='monospace',
            bbox=dict(boxstyle='round', facecolor='lightblue', alpha=0.3))

    plt.tight_layout()
    plt.savefig(output_dir / 'benchmark_summary.png', dpi=300, bbox_inches='tight')
    print(f"Saved: {output_dir / 'benchmark_summary.png'}")
    plt.close()

    print("\nAll visualizations created successfully!")

if __name__ == '__main__':
    import sys

    # Default to benchmark_results.json in current directory
    results_file = sys.argv[1] if len(sys.argv) > 1 else 'benchmark_results.json'
    
    if not Path(results_file).exists():
        print(f"Error: {results_file} not found")
        sys.exit(1)

    results = load_results(results_file)
    create_visualizations(results)
