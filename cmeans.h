#ifndef CMEANS_H
#define CMEANS_H

#include "point.h"
#include <QVector>

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
