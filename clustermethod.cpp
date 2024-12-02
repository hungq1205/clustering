#include "clustermethod.h"
#include "QDebug"

double ClusterMethod::euclideanDistance(const Point& a, const Point& b) {
    if (a.x.size() != b.x.size())
        throw ("Unequivalent points");

    double o = 0.0;
    int l = a.x.size();
    for (int i = 0; i < l; i++) {
        double d = a.x[i] - b.x[i];
        o += d * d;
    }
    return sqrt(o);
}

double ClusterMethod::entropyDistance(const Point& a, const Point& b) {
    if (a.x.size() != b.x.size())
        throw ("Unequivalent points");

    double o = 0.0;
    int l = a.x.size();
    for (int i = 0; i < l; i++) {
        double d = a.x[i] - b.x[i];
        o += d * d;
    }
    return sqrt(o);
}
