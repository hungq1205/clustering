#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmeans.h"
#include "sscmeans.h"
#include "QFileDialog"
#include "datautility.h"
#include <QRandomGenerator>

SupervisorWindow *MainWindow::supervisorw = nullptr;
QMap<Point, QList<double>> *MainWindow::supervisedData;
QVector<Point*> *MainWindow::points, *MainWindow::inferPoints;
QList<bool> *MainWindow::inferables;
QVector<DatapointView*> *MainWindow::pointViews;
QStringList *MainWindow::xlabels;
QStringList *MainWindow::ylabels;
int MainWindow::distanceMetricIdx = 0;
int MainWindow::xfield = 0;
int MainWindow::yfield = 0;
QString MainWindow::projDir;
ClusterMethod *MainWindow::cmethod;

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

    xlabels = new QStringList();
    xlabels->append("x");
    xlabels->append("y");
    ui->labelCBoxA->addItems(*xlabels);
    ui->labelCBoxB->addItems(*xlabels);
    ui->labelCBoxB->setCurrentIndex(1);

    ylabels = new QStringList();
    ylabels->append("1");
    ylabels->append("2");
    ylabels->append("3");

    ui->dataViewContainer->setVisible(false);

    pointViews = new QVector<DatapointView*>;
    points = new QVector<Point*>;
    inferPoints = new QVector<Point*>;
    inferables = new QList<bool>;
    inferables->append(true);
    inferables->append(true);

    supervisedData = new QMap<Point, QList<double>>();
    supervisorw = new SupervisorWindow;
    supervisorw->InitFields(3, *ylabels);
    DatapointView::xValSupervisor = new SupervisorWindow();
    DatapointView::yValSupervisor = new SupervisorWindow();
    distanceMetricIdx = 0;
    xfield = ui->labelCBoxA->currentIndex();
    yfield = ui->labelCBoxB->currentIndex();
    maxIters = ui->iter_input->value();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SwitchDisplayMode(bool isColorMap) {
    this->isColorMap = isColorMap;
    ui->colormap->setVisible(isColorMap);
    ui->dataViewContainer->setVisible(!isColorMap);
}

void MainWindow::writePoint(int idx, const QList<double>& val) {
    int j = 0;
    for (int i = 0; i < val.size(); i++)
    {
        (*points)[idx]->x[i] = val[i];
        if ((*inferables)[i])
        {
            (*inferPoints)[idx]->x[j] = val[i];
            j++;
        }
    }
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

    cmethod->fit(*inferPoints, maxIters);

    if (isColorMap) {
        int w = ui->colormap->width();
        int h = ui->colormap->height();
        xfield = ui->labelCBoxA->currentIndex();
        yfield = ui->labelCBoxB->currentIndex();
        int n = (*inferPoints)[0]->x.size();
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
    else {
        for (auto v : *pointViews)
            v->UpdateLabel();
    }
}

void MainWindow::on_button_clicked() {
    fit();
}

void MainWindow::on_pointClearBtn_clicked() {
    supervisedData->clear();
    points->clear();
    inferPoints->clear();
    pointViews->clear();
    SwitchDisplayMode(true);
    ui->colormap->render();
}

void MainWindow::on_cluster_input_valueChanged(int clusterNum) {
    delete supervisorw;
    supervisorw = new SupervisorWindow();
    supervisorw->InitFields(clusterNum, *ylabels);
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
        xlabels = &std::get<1>(*it);
        ui->labelCBoxA->clear();
        ui->labelCBoxA->addItems(*xlabels);
        ui->labelCBoxB->clear();
        ui->labelCBoxB->addItems(*xlabels);
        ui->labelCBoxB->setCurrentIndex(1);
        xfield = ui->labelCBoxA->currentIndex();
        yfield = ui->labelCBoxB->currentIndex();

        for (auto v : *pointViews)
            v->deleteLater();
        pointViews->clear();

        ylabels->clear();
        delete ylabels;
        ylabels = new QStringList();
        for (int i = 0; i < ui->cluster_input->value(); i++)
            ylabels->append(QString::number(i + 1));
        supervisorw->InitFields(ylabels->size(), *ylabels);
        DatapointView::xValSupervisor->InitFields(xlabels->size(), *xlabels);
        DatapointView::yValSupervisor->InitFields(ylabels->size(), *ylabels, true);

        delete inferables;
        inferables = &std::get<0>(*it);

        for (auto p : *inferPoints)
            delete p;
        delete inferPoints;
        inferPoints = &std::get<2>(*it);

        for (auto p : *points)
            delete p;
        delete points;
        points = &std::get<3>(*it);

        SwitchDisplayMode(false);
        for (int i = 0; i < points->size(); i++) {
            DatapointView *pv = new DatapointView(this);
            ui->dataviewList->addWidget(pv);
            pointViews->append(pv);
            pv->SetPointIndex(i);
            pv->SetColor(i % 2 == 0);
        }
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
        auto sdata = DataUtility::readSupervisedData(fn);
        supervisedData->clear();
        ylabels->clear();
        ylabels = &std::get<0>(*sdata);
        supervisedData = std::get<1>(*sdata);
        supervisorw->InitFields(ylabels->size(), *ylabels);
        DatapointView::yValSupervisor->InitFields(ylabels->size(), *ylabels, true);

        for (auto v : *pointViews)
            v->UpdateSupervisedData();
    }
}

void MainWindow::on_exportBtn_clicked() {
    fit();
    DataUtility::writeData(projDir + "/output/centroids.txt", cmethod->centroids);
}


void MainWindow::on_labelCBoxA_currentIndexChanged(int index)
{
    xfield = index;
    if (!isColorMap)
    {
        for (auto v : *pointViews)
            v->UpdateData();
    }
}


void MainWindow::on_labelCBoxB_currentIndexChanged(int index)
{
    yfield = index;
    if (!isColorMap)
    {
        for (auto v : *pointViews)
            v->UpdateData();
    }
}

