#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmeans.h"
#include "sscmeans.h"
#include "QFileDialog"
#include "datautility.h"
#include <QRandomGenerator>

SupervisorWindow *MainWindow::supervisorw = nullptr;
QMap<Point, QList<double>> *MainWindow::supervisedData;
QStringList *MainWindow::labels;
int MainWindow::distanceMetricIdx = 0;
int MainWindow::xfield = 0;
int MainWindow::yfield = 0;
QString MainWindow::projDir;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    projDir = dir.absolutePath();

    ui->setupUi(this);
    ui->colormap->setRegionNum(3);

    ui->clusterMethod->addItem("CMeans");
    ui->clusterMethod->addItem("Semi-Supervised CMeans");

    ui->distanceMetric->addItem("Euclidean");
    ui->distanceMetric->addItem("Entropy");

    labels = new QStringList();
    labels->append("x");
    labels->append("y");
    ui->labelCBoxA->addItems(*labels);
    ui->labelCBoxB->addItems(*labels);
    ui->labelCBoxB->setCurrentIndex(1);

    supervisedData = new QMap<Point, QList<double>>();
    supervisorw = new SupervisorWindow;
    supervisorw->InitFields(3);
    distanceMetricIdx = 0;
    xfield = ui->labelCBoxA->currentIndex();
    yfield = ui->labelCBoxB->currentIndex();
    maxIters = ui->iter_input->value();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::fit() {
    double m = ui->m_input->value();
    if (m <= 1)
        m = 1.00001;

    switch (ui->clusterMethod->currentIndex()) {
    case 0:
        cmethod = new CMeans(ui->cluster_input->value(), m);
        break;
    case 1:
        cmethod = new SSCMeans(ui->cluster_input->value(), m, *supervisedData);
        break;
    }

    cmethod->fit(*(ui->colormap->points), maxIters);

    int w = ui->colormap->width();
    int h = ui->colormap->height();
    xfield = ui->labelCBoxA->currentIndex();
    yfield = ui->labelCBoxB->currentIndex();
    int n = (*ui->colormap->points)[0]->x.size();
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            QList<double> px(n);
            px[xfield] = i;
            px[yfield] = j;
            QVector<double> enc = cmethod->predict(Point(px));
            ui->colormap->setPixel(i, j, enc);
        }

    ui->colormap->render();
}

void MainWindow::on_button_clicked() {
    fit();
}

void MainWindow::on_pointClearBtn_clicked() {
    supervisedData->clear();
    ui->colormap->points->clear();
    ui->colormap->render();
}

void MainWindow::on_cluster_input_valueChanged(int clusterNum) {
    delete supervisorw;
    supervisorw = new SupervisorWindow();
    supervisorw->InitFields(clusterNum);
    ui->colormap->setRegionNum(clusterNum);
}

void MainWindow::on_iter_input_valueChanged(int iters) {
    maxIters = iters;
}

void MainWindow::on_distanceMetric_currentIndexChanged(int index) {
    distanceMetricIdx = index;
}

void MainWindow::on_clusterMethod_currentIndexChanged(int index) {
    if (index == 1) {
        ui->supervisedDataBrowser->setEnabled(true);
        ui->superviseBtn->setEnabled(true);
    }
    else {
        ui->supervisedDataBrowser->setEnabled(false);
        ui->superviseBtn->setChecked(false);
        ui->superviseBtn->setEnabled(false);
    }
}

void MainWindow::on_superviseBtn_toggled(bool checked) {
    ColorMap::isSupervising = checked;
}

void MainWindow::on_dataBrowser_clicked() {
    QFileDialog dialog(this);
    dialog.setWindowTitle("Choose labeled data file");
    dialog.setDirectory(projDir);
    dialog.setNameFilter("Text (*.txt);;All Files (*)");
    dialog.setFileMode(QFileDialog::ExistingFile);

    if (dialog.exec()) {
        QString fn = dialog.selectedFiles().constFirst();
        ui->dataPath->setText(fn);
        auto it = DataUtility::readData(fn);
        labels = &std::get<0>(*it);
        ui->labelCBoxA->clear();
        ui->labelCBoxA->addItems(*labels);
        ui->labelCBoxB->clear();
        ui->labelCBoxB->addItems(*labels);
        ui->labelCBoxB->setCurrentIndex(1);
        xfield = ui->labelCBoxA->currentIndex();
        yfield = ui->labelCBoxB->currentIndex();

        delete ui->colormap->points;
        ui->colormap->points = &std::get<1>(*it);
    }
}

void MainWindow::on_supervisedDataBrowser_clicked() {
    QFileDialog dialog(this);
    dialog.setWindowTitle("Choose unlabeled data file");
    dialog.setDirectory(projDir);
    dialog.setNameFilter("Text (*.txt);;All Files (*)");
    dialog.setFileMode(QFileDialog::ExistingFile);

    if (dialog.exec()) {
        QString fn = dialog.selectedFiles().constFirst();
        ui->supervisedDataPath->setText(fn);
        delete supervisedData;
        supervisedData = DataUtility::readSupervisedData(fn);
    }
}

void MainWindow::on_exportBtn_clicked() {
    fit();
    DataUtility::writeData(projDir + "/output/centroids.txt", cmethod->centroids);
}

