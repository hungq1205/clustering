#ifndef CMEANS_H
#define CMEANS_H

#include "clustermethod.h"

class CMeans : public ClusterMethod
{
public:
    CMeans(int c, double m);
    void fit(QList<Point*>& data, int maxIters, double epsilon=1e-6);
    QList<double> predict(const Point& p);
private:
    int c;
    double m;

    double getDistance(const Point& a, const Point& b);
};

#endif // CMEANS_H
