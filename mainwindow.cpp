#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cmeans(CMeans(3, 3))
{
    ui->setupUi(this);
    ui->colormap->setRegionNum(3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_clicked()
{
    cmeans.fit(ui->colormap->points, maxIters);

    int w = ui->colormap->width();
    int h = ui->colormap->height();
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            QVector<double> enc = cmeans.predict(Point(i, j));
            ui->colormap->setPixel(i, j, enc);
        }

    ui->colormap->render();
}


void MainWindow::on_pointClearBtn_clicked()
{
    ui->colormap->points.clear();
    ui->colormap->render();
}


void MainWindow::on_cluster_input_valueChanged(int clusterNum)
{
    cmeans = CMeans(clusterNum, ui->m_input->value());
    ui->colormap->setRegionNum(clusterNum);
}


void MainWindow::on_iter_input_valueChanged(int iters)
{
    maxIters = iters;
}


void MainWindow::on_m_input_valueChanged(double m)
{
    if (m <= 1)
        m = 1.000001;
    cmeans = CMeans(ui->cluster_input->value(), m);
}

