#pragma once

#include <vector>
#include <array>
#include <tuple>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <stdexcept>

struct Point {
    double x, y;

    Point();
    Point(double x, double y);
    Point(const Point& point);
    Point& operator=(const Point& point);
    Point& operator/=(double num);
    Point operator-(const Point& point) const;
};

class Coordinates {
public:
    std::tuple<double, double, std::vector<Point>, std::vector<Point>> fitLineRansac(size_t iterations = 1000, double sigma = 1.0, double k_min = -7.0, double k_max = 7.0);
    const Point& operator[](size_t i);
    std::vector<Point> getPoints();
    size_t size() const noexcept;
    double norm(const Point& point);
    void push_back(const Point& point);
    void push_back(Point&& point);
    void clear();
private:
    std::vector<Point> points;
};
