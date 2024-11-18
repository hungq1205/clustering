#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cmeans.h"
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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_button_clicked();
    void on_pointClearBtn_clicked();

    void on_cluster_input_valueChanged(int cluster);

    void on_iter_input_valueChanged(int iters);

    void on_m_input_valueChanged(double m);

private:
    Ui::MainWindow *ui;
    CMeans cmeans;
    int maxIters = 250;
};
#endif // MAINWINDOW_H
