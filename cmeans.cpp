#include "cmeans.h"
#include <QRandomGenerator>

CMeans::CMeans(int c, double m) : c(c), m(m) {}

double CMeans::distanceOf(const Point& p1, const Point& p2) {
    double xdif = p1.x - p2.x, ydif = p1.y - p2.y;
    return sqrt(xdif * xdif + ydif * ydif);
}

void CMeans::fit(QVector<Point>& data, int maxIters, double epsilon) {
    int n = data.size();

    // Init membership matrix
    QVector<QVector<double>> U(n, QVector<double>(c));
    auto rand = QRandomGenerator::global();
    for (int i = 0; i < n; ++i) {
        double sum = 0;
        for (int j = 0; j < c; ++j) {
            U[i][j] = rand->generate() % 500;
            sum += U[i][j];
        }

        for (int j = 0; j < c; ++j)
            U[i][j] /= sum;
    }

    // Update
    centroids = QVector<Point>(c);
    for (int iter = 0; iter < maxIters; ++iter) {
        QVector<double> denom(c, 0.0);

        for (int j = 0; j < c; ++j) {
            double sumX = 0.0, sumY = 0.0;
            for (int i = 0; i < n; ++i) {
                double u_ij = pow(U[i][j], m);
                sumX += u_ij * data[i].x;
                sumY += u_ij * data[i].y;
                denom[j] += u_ij;
            }
            centroids[j] = Point(sumX / denom[j], sumY / denom[j]);
        }

        if (iter == maxIters - 1)
            break;

        for (int i = 0; i < n; ++i)
            U[i] = predict(data[i]);
    }
}

QVector<double> CMeans::predict(const Point& p) {
    QVector<double> output(c);

    for (int i = 0; i < c; ++i) {
        double sum = 0.0;
        double idist = distanceOf(p, centroids[i]);
        for (int j = 0; j < c; ++j) {
            double dist = idist / distanceOf(p, centroids[j]);
            sum += pow(dist, 2.0 / (m - 1));
        }
        output[i] = 1.0 / sum;
    }

    return output;
}
