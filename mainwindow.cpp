#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmeans.h"
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_clicked()
{
    int cluster = 3;
    double m = 3;
    int dataCount = 100;
    int maxIterations = 100;

    ui->colormap->setRegionNum(cluster);

    int w = ui->colormap->width();
    int h = ui->colormap->height();
    auto rand = QRandomGenerator::global();
    QVector<Point> data{};
    for (int i = 0; i < dataCount; i++)
        data.append(Point(rand->generate() % w, rand->generate() % h));

    CMeans cmeans(cluster, m);
    cmeans.fit(data, maxIterations);

    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            QVector<double> enc = cmeans.predict(Point(i, j));
            ui->colormap->setPixel(i, j, enc);
        }

    for (auto p : data)
        ui->colormap->setPixel(p.x, p.y, Qt::white);

    ui->colormap->render();
}

