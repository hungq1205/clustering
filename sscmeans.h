#ifndef SSCMEANS_H
#define SSCMEANS_H

#include "clustermethod.h"
#include "QMap"

class SSCMeans : public ClusterMethod
{
public:
    SSCMeans(int c, double m, QMap<Point, QList<double>>& supervised);
    void fit(QList<Point*>& data, int maxIters, double epsilon=1e-6);
    QList<double> predict(const Point& p);
private:
    int c;
    double m;
    QMap<Point, QList<double>> supervised;

    double getDistance(const Point& a, const Point& b);
};

#endif // SSCMEANS_H
