#include "datautility.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

void DataUtility::writeData(const QString &fname, const QVector<Point*> &data) {
    QFile file(fname);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening file for writing:" << file.errorString();
        return;
    }

    QTextStream out(&file);

    for (auto& p : data) {
        QStringList line;
        for (double val : p->x)
            line.append(QString::number(val, 'f', 2));
        out << line.join(", ") << "\n";
    }
    file.close();
}

std::tuple<QList<bool>, QStringList, QVector<Point*>, QVector<Point*>>* DataUtility::readData(const QString &fname) {
    QFile file(fname);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file: " << file.errorString();
        return nullptr;
    }
    QTextStream in(&file);

    QString inferLine = in.readLine();
    QStringList rawInfers = inferLine.split(",", Qt::KeepEmptyParts);
    QList<bool> inferables;
    for (auto it = rawInfers.begin(); it != rawInfers.end(); it++)
    {
        if (it->toStdString() == "0")
            inferables.append(false);
        else
            inferables.append(true);
    }

    QString labelLine = in.readLine();
    QStringList labels = labelLine.split(",", Qt::KeepEmptyParts);

    QVector<Point*> data, dataInfer;
    while (!in.atEnd()) {
        QStringList items = in.readLine().split(",", Qt::KeepEmptyParts);
        QVector<double> p;
        QVector<double> pInfer;
        int i = 0;
        for (const QString &it : items) {
            auto val = it.toDouble();
            p.append(val);
            if (inferables[i])
                pInfer.append(val);
            i++;
        }
        data.append(new Point(p));
        dataInfer.append(new Point(pInfer));
    }
    file.close();

    return new std::tuple(inferables, labels, dataInfer, data);
}

 std::tuple<QStringList, QMap<Point, QList<double>>*>* DataUtility::readSupervisedData(const QString &fname) {
    QFile file(fname);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file: " << file.errorString();
        return nullptr;
    }

    QTextStream in(&file);

    QString labelLine = in.readLine();
    QStringList labels = labelLine.split(",", Qt::KeepEmptyParts);

    QMap<Point, QList<double>>* data = new QMap<Point, QList<double>>();
    while (!in.atEnd()) {
        QStringList items = in.readLine().split(",", Qt::KeepEmptyParts);
        QVector<double> px;
        for (const QString &it : items) {
            px.append(it.toDouble());
        }

        items = in.readLine().split(",", Qt::KeepEmptyParts);
        QVector<double> c;
        for (const QString &it : items) {
            c.append(it.toDouble());
        }

        data->insert(Point(px), c);
    }
    file.close();

    return new std::tuple(labels, data);
}
