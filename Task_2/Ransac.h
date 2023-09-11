#pragma once

#include <vector>
#include <array>
#include <cmath>
#include <stdlib.h>
#include <time.h>

struct Point {
    double x, y;

    Point();
    Point(double x, double y);
    Point(const Point& point);
    Point& operator/=(double num);
};

Point operator-(const Point& point_1, const Point& point_2);

double norm(const Point& point);

std::pair<double, double> fitLineRansac(const std::vector<Point>& points, int iterations = 1000, double sigma = 1.0, double k_min = -7.0, double k_max = 7.0);
