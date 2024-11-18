#ifndef CMEANS_H
#define CMEANS_H

#include <QVector>

struct Point {
    double x, y;
    Point() : x(0.0), y(0.0) {}
    Point(double x, double y) : x(x), y(y) {}
};

class CMeans
{
public:
    QVector<Point> centroids;

    CMeans(int c, double m);
    double distanceOf(const Point& a, const Point& b);
    void fit(QVector<Point>& data, int maxIters, double epsilon=1e-6);
    QVector<double> predict(const Point& p);
private:
    int c;
    double m;
};

#endif // CMEANS_H
