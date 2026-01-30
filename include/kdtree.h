#pragma once

#include "point.h"
#include "grid.h"
#include <vector>
#include <memory>
#include <algorithm>

struct KDNode {
    Point point;
    Cell cell;
    std::unique_ptr<KDNode> left;
    std::unique_ptr<KDNode> right;

    KDNode(const Point& p, const Cell& c)
        : point(p), cell(c), left(nullptr), right(nullptr) {}
};

class KDTree {
private:
    std::unique_ptr<KDNode> root;
    size_t node_count;

    std::unique_ptr<KDNode> buildTree(std::vector<std::pair<Point, Cell>>& points,
                                      int depth, int start, int end) {
        if (start >= end) return nullptr;

        int axis = depth % 2;  // 0 for x, 1 for y
        int mid = start + (end - start) / 2;

        std::nth_element(points.begin() + start, points.begin() + mid,
                        points.begin() + end,
                        [axis](const auto& a, const auto& b) {
                            if (axis == 0) return a.first.x < b.first.x;
                            return a.first.y < b.first.y;
                        });

        auto node = std::make_unique<KDNode>(points[mid].first, points[mid].second);
        node->left = buildTree(points, depth + 1, start, mid);
        node->right = buildTree(points, depth + 1, mid + 1, end);

        return node;
    }

    Cell searchHelper(const KDNode* node, const Point& target, int depth) const {
        if (!node) return Cell(0, 0, Point(0, 0), 0);

        int axis = depth % 2;
        Cell best = node->cell;

        const KDNode* next_branch = nullptr;
        const KDNode* opposite_branch = nullptr;

        if (axis == 0) {  // x axis
            if (target.x < node->point.x) {
                next_branch = node->left.get();
                opposite_branch = node->right.get();
            } else {
                next_branch = node->right.get();
                opposite_branch = node->left.get();
            }
        } else {  // y axis
            if (target.y < node->point.y) {
                next_branch = node->left.get();
                opposite_branch = node->right.get();
            } else {
                next_branch = node->right.get();
                opposite_branch = node->left.get();
            }
        }

        // Search the next branch
        if (next_branch) {
            Cell candidate = searchHelper(next_branch, target, depth + 1);
            if (candidate.contains(target)) {
                best = candidate;
            }
        }

        // Check if we need to search the opposite branch
        double axis_dist = (axis == 0) ? std::abs(target.x - node->point.x)
                                       : std::abs(target.y - node->point.y);

        if (opposite_branch && axis_dist <= best.half_size) {
            Cell candidate = searchHelper(opposite_branch, target, depth + 1);
            if (candidate.contains(target)) {
                best = candidate;
            }
        }

        return best;
    }

public:
    KDTree() : node_count(0) {}

    void build(std::vector<std::pair<Point, Cell>>& points) {
        std::vector<std::pair<Point, Cell>> temp = points;
        root = buildTree(temp, 0, 0, temp.size());
        node_count = points.size();
    }

    Cell search(const Point& target) const {
        if (!root) return Cell(0, 0, Point(0, 0), 0);
        return searchHelper(root.get(), target, 0);
    }

    size_t getNodeCount() const { return node_count; }
};
