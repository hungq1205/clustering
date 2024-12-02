#ifndef CLUSTER_METHOD_H
#define CLUSTER_METHOD_H

#include "point.h"
#include <QVector>

class ClusterMethod
{
public:
    QList<Point*> centroids;

    static double euclideanDistance(const Point& a, const Point& b);
    static double entropyDistance(const Point& a, const Point& b);

    virtual void fit(QList<Point*>& data, int maxIters, double epsilon=1e-6) = 0;
    virtual QList<double> predict(const Point& p) = 0;
};

#endif // CLUSTER_METHOD_H
