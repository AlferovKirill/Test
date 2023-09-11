#include "Interface.h"
#include "Ransac.h"

#include <QApplication>

int main(int argc, char** argv) {
    std::vector<Point> points;

    points.push_back(Point(0, 3));
    points.push_back(Point(-6, -1));
    points.push_back(Point(9, 9));
    points.push_back(Point(12, 11));

    auto pair = fitLineRansac(points, 1000, 10);

    Point p1, p2;

    p1.y = 720;
    p1.x = (p1.y - pair.second) / pair.first;
    p2.y = 360;
    p2.x = (p2.y - pair.second) / pair.first;

    qDebug() << "-> " << p1.x << " " << p1.y;
    qDebug() << "-> " << p2.x << " " << p2.y;
    qDebug() << "-> k = " << pair.first << " b = " << pair.second;

    return 0;

    //QApplication a(argc, argv);
    //Interface w;
    //w.show();
    //return a.exec();
}
