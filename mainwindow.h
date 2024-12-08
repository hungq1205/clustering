#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "clustermethod.h"
#include "supervisorwindow.h"
#include "datapointview.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static SupervisorWindow *supervisorw;
    static QMap<Point, QList<double>>* supervisedData;
    static QVector<DatapointView*> *pointViews;
    static QVector<Point*> *points;
    static QVector<Point*> *inferPoints;
    static QList<bool> *inferables;
    static QStringList* xlabels;
    static QStringList* ylabels;
    static int distanceMetricIdx;
    static int xfield, yfield;
    static ClusterMethod *cmethod;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SwitchDisplayMode(bool isColorMap);

    static void writePoint(int idx, const QList<double>& val);

private slots:
    void on_button_clicked();

    void on_pointClearBtn_clicked();

    void on_cluster_input_valueChanged(int cluster);

    void on_iter_input_valueChanged(int iters);

    void on_distanceMetric_currentIndexChanged(int index);

    void on_clusterMethod_currentIndexChanged(int index);

    void on_superviseBtn_toggled(bool checked);

    void on_dataBrowser_clicked();

    void on_supervisedDataBrowser_clicked();

    void on_exportBtn_clicked();

    void on_labelCBoxA_currentIndexChanged(int index);

    void on_labelCBoxB_currentIndexChanged(int index);

private:
    static QString projDir;
    Ui::MainWindow *ui;
    int maxIters = 250;
    bool isColorMap = true;

    void fit();
};
#endif // MAINWINDOW_H
