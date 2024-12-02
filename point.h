#ifndef POINT_H
#define POINT_H

#include "QVector"

class Point {
public:
    QList<double> x;

    Point() : x(QList<double>()) {}
    Point(const QList<double>& x) : x(x) {}
    Point(int a, int b) {
        x = QList<double>(2);
        x[0] = a;
        x[1] = b;
    }

    bool operator<(const Point& other) const {
        return x < other.x;
    }
};

#endif // POINT_H
