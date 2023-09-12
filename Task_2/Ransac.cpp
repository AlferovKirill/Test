#include "Ransac.h"

Point::Point() = default;

Point::Point(double x, double y) : x(x), y(y) {}

Point::Point(const Point& point) {
    x = point.x;
    y = point.y;
}

Point& Point::operator=(const Point& point) {
    x = point.x;
    y = point.y;

    return *this;
}

Point& Point::operator/=(double num) {
    x /= num;
    y /= num;

    return *this;
}

Point Point::operator-(const Point& point) const {
    return Point(x - point.x, y - point.y);
}

double norm(const Point& point) {
    return std::sqrt(point.x * point.x + point.y * point.y);
}

std::tuple<double, double, std::vector<Point>> fitLineRansac(const std::vector<Point>& points, int iterations, double sigma, double k_min, double k_max) {
    size_t n = points.size();

    if(n == 2) {
        double k = points[0].x / points[0].y;
        double b = points[1].x - k * points[1].y;

        return std::tuple<double, double, std::vector<Point>>(k, b, points);
    }
    else if (n < 2) {
        return std::tuple<double, double, std::vector<Point>>(0, 0, points);
    }

    std::array<double, 4> line;
    std::vector<Point> goodPoints;

    srand(time(NULL));

    double bestScore = -1.0;

    for(int k = 0; k < iterations; k++) {
        int i1 = 0, i2 = 0;

        while(i1 == i2) {
            i1 = rand() % points.size();
            i2 = rand() % points.size();
        }

        const Point& p1 = points[i1];
        const Point& p2 = points[i2];

        std::vector<Point> currentPoints;

        Point dp = p2 - p1;
        dp /= norm(dp);
        double score = 0;

        if(dp.y / dp.x <= k_max && dp.y / dp.x >= k_min ) {
            for(size_t i = 0; i < n; ++i) {
                Point v = points[i] - p1;
                double d = v.y * dp.x - v.x * dp.y;

                if(fabs(d) < sigma) {
                    ++score;
                    currentPoints.push_back(points[i]);
                }
            }
        }

        if(score > bestScore) {
            line = { dp.x, dp.y, p1.x, p1.y };
            bestScore = score;

            goodPoints = currentPoints;
        }
    }

    double k = line[1] / line[0];
    double b = line[3] - k * line[2];

    return std::tuple<double, double, std::vector<Point>>(k, b, goodPoints);
}

std::vector<Point> Coordinates::getPoints() {
    return points;
}

void Coordinates::push_back(const Point& point) {
    points.push_back(point);
}

void Coordinates::push_back(Point&& point) {
    points.push_back(std::move(point));
}

size_t Coordinates::size() const noexcept {
    return points.size();
}

void Coordinates::clear() {
    points.clear();
}
