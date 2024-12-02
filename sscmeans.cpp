#include "sscmeans.h"
#include "QRandomGenerator"
#include "mainwindow.h"

SSCMeans::SSCMeans(int c, double m, QMap<Point, QList<double>>& supervised) {
    this->c = c;
    this->m = m;
    this->supervised = supervised;
}

void SSCMeans::fit(QList<Point*>& data, int maxIters, double epsilon) {
    int n = data.size();
    int dim = data[0]->x.size();

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
    centroids = QList<Point*>(c);
    for (int iter = 0; iter < maxIters; ++iter) {
        QList<double> denom(c, 0.0);

        for (int j = 0; j < c; ++j) {
            QList<double> cen(dim);
            for (int i = 0; i < n; ++i) {
                double u_ij = pow(U[i][j], m);
                for (int d = 0; d < dim; d++)
                    cen[d] += u_ij * data[i]->x[d];
                denom[j] += u_ij;
            }
            for (int d = 0; d < dim; d++)
                cen[d] /= denom[j];
            centroids[j] = new Point(cen);
        }

        if (iter == maxIters - 1)
            break;

        for (int i = 0; i < n; ++i)
            U[i] = predict(*data[i]);
    }
}

QList<double> SSCMeans::predict(const Point& p) {
    bool sfound = false;
    QList<double> output(c);
    QList<double>* sdata;

    auto it = supervised.find(p);
    if (it != supervised.end()) {
        sdata = &it.value();
        sfound = true;
    }

    double u_comp = 1.0;
    if (sfound)
        for (int i = 0; i < c; ++i)
            u_comp -= (*sdata)[i];

    for (int i = 0; i < c; ++i) {
        double sum = 0.0;
        double idist = getDistance(p, *(centroids[i]));
        for (int j = 0; j < c; ++j) {
            double dist = idist / getDistance(p, *(centroids[j]));
            sum += pow(dist, 1.0 / (m - 1));
        }
        if (sfound)
            output[i] = (*sdata)[i] + u_comp / sum;
        else
            output[i] = u_comp / sum;
    }

    return output;
}

double SSCMeans::getDistance(const Point& a, const Point& b) {
    switch (MainWindow::distanceMetricIdx) {
    case 0: return euclideanDistance(a, b);
    case 1: return entropyDistance(a, b);
    }

    return -1;
}
