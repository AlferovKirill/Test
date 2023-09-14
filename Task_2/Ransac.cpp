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

double Coordinates::norm(const Point& point) {
    return std::sqrt(point.x * point.x + point.y * point.y);
}

std::tuple<double, double, std::vector<Point>, std::vector<Point>> Coordinates::fitLineRansac(size_t iterations, double sigma, double k_min, double k_max) {
    double k, b, bestScore = -1.0;

    std::array<double, 4> line;
    std::vector<Point> inlierPoints;
    std::vector<Point> outlierPoints;

    srand(time(NULL));

    if(points.size() == 2) {
        if (points[0].y / points[0].x != points[1].y / points[1].x) {
            k = (points[0].y - points[1].y) / (points[0].x - points[1].x);
            b = points[0].y - k * points[0].x;
        }
        else {
            b = 0;
            k = points[0].y / points[0].x;
        }

        return std::tuple<double, double, std::vector<Point>, std::vector<Point>>(k, b, points, {});
    }
    else if (points.size() < 2) {
        throw std::runtime_error("Nothing to fit");
    }

    for(size_t k = 0; k < iterations; k++) {
        int i1 = 0, i2 = 0;

        while(i1 == i2) {
            i1 = rand() % points.size();
            i2 = rand() % points.size();
        }

        const Point& p1 = points[i1];
        const Point& p2 = points[i2];

        std::vector<Point> currentInlierPoints;
        std::vector<Point> currentOutlierPoints;

        Point dp = p2 - p1;
        dp /= norm(dp);
        double score = 0;

        if(dp.y / dp.x <= k_max && dp.y / dp.x >= k_min ) {
            for(size_t i = 0; i < points.size(); ++i) {
                Point v = points[i] - p1;
                double d = v.y * dp.x - v.x * dp.y;

                if(fabs(d) < sigma) {
                    ++score;
                    currentInlierPoints.push_back(points[i]);
                }
                else {
                    currentOutlierPoints.push_back(points[i]);
                }
            }
        }

        if(score > bestScore) {
            line = { dp.x, dp.y, p1.x, p1.y };
            bestScore = score;
            inlierPoints = currentInlierPoints;
            outlierPoints = currentOutlierPoints;
        }
    }

    k = line[1] / line[0];
    b = line[3] - k * line[2];

    return std::tuple<double, double, std::vector<Point>, std::vector<Point>>(k, b, inlierPoints, outlierPoints);
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
