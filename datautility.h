#ifndef DATAUTILITY_H
#define DATAUTILITY_H

#include "point.h"
#include <tuple>

class DataUtility
{
public:
    static void writeData(const QString &fname, const QVector<Point*> &data);
    static std::tuple<QList<bool>, QStringList, QVector<Point*>, QVector<Point*>>* readData(const QString &fname);
    static std::tuple<QStringList, QMap<Point, QList<double>>*>* readSupervisedData(const QString &fname);
private:
    DataUtility() = delete;
};

#endif // DATAUTILITY_H
