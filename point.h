#ifndef POINT_H
#define POINT_H

struct Point {
    double x, y;
    Point() : x(0.0), y(0.0) {}
    Point(double x, double y) : x(x), y(y) {}
};

#endif // POINT_H
