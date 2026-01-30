#pragma once

#include "point.h"
#include <cmath>

struct Cell {
    int grid_x;  // grid cell index
    int grid_y;  // grid cell index
    Point center;
    double half_size;  // half of cell width/height

    Cell(int gx, int gy, Point c, double hs)
        : grid_x(gx), grid_y(gy), center(c), half_size(hs) {}

    bool contains(const Point& p) const {
        return std::abs(p.x - center.x) <= half_size &&
               std::abs(p.y - center.y) <= half_size;
    }

    bool operator==(const Cell& other) const {
        return grid_x == other.grid_x &&
               grid_y == other.grid_y &&
               center == other.center &&
               std::abs(half_size - other.half_size) < 1e-9;
    }
};

class Grid {
private:
    int grid_size;  // NxN grid
    double max_coord;  // coordinate system range [0, max_coord]
    double cell_width;  // width of each cell
    double cell_height;  // height of each cell

public:
    Grid(int size, double max_coord)
        : grid_size(size), max_coord(max_coord) {
        cell_width = max_coord / grid_size;
        cell_height = max_coord / grid_size;
    }

    Cell getCellAtPoint(const Point& p) const {
        // Find which cell contains this point
        int grid_x = static_cast<int>(p.x / cell_width);
        int grid_y = static_cast<int>(p.y / cell_height);

        // Clamp to grid bounds
        grid_x = std::min(std::max(grid_x, 0), grid_size - 1);
        grid_y = std::min(std::max(grid_y, 0), grid_size - 1);

        Point center(
            (grid_x + 0.5) * cell_width,
            (grid_y + 0.5) * cell_height
        );

        return Cell(grid_x, grid_y, center, cell_width / 2.0);
    }

    int getGridSize() const { return grid_size; }
    double getMaxCoord() const { return max_coord; }
    double getCellWidth() const { return cell_width; }
    double getCellHeight() const { return cell_height; }
};
