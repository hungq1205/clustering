#include "datapointview.h"
#include "ui_datapointview.h"
#include "mainwindow.h"
#include "point.h"

SupervisorWindow *DatapointView::yValSupervisor, *DatapointView::xValSupervisor;

DatapointView::DatapointView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DatapointView)
{
    ui->setupUi(this);
}

DatapointView::~DatapointView()
{
    delete ui;
}

void DatapointView::SetPointIndex(int index) {
    pointIdx = index;
    UpdateData();
}

void DatapointView::SetColor(bool odd) {
    if (odd)
        setStyleSheet("#Container{background-color: rgba(0, 0, 0, 10)}");
    else
        setStyleSheet("#Container{background-color: rgba(0, 0, 0, 20)}");
    show();
}

void DatapointView::UpdateData() {
    if (pointIdx < 0)
    {
        ui->xVal->setText(QString::number(0));
        ui->yVal->setText(QString::number(0));
    }
    else
    {
        ui->xLab->setText(((*MainWindow::xlabels)[MainWindow::xfield]));
        ui->yLab->setText(((*MainWindow::xlabels)[MainWindow::yfield]));
        ui->xVal->setText(QString::number((*MainWindow::points)[pointIdx]->x[MainWindow::xfield]));
        ui->yVal->setText(QString::number((*MainWindow::points)[pointIdx]->x[MainWindow::yfield]));
    }
}

void DatapointView::UpdateSupervisedData() {

}

void DatapointView::UpdateLabel() {
    auto c = MainWindow::cmethod->predict(*(*MainWindow::inferPoints)[pointIdx]);

    double max = 0;
    for (int i = 0; i < c.size(); i++)
        if (c[i] > c[max])
            max = i;
    ui->clusterVal->setText((*MainWindow::ylabels)[max]);
}

void DatapointView::on_supervisedbtn_clicked()
{
    if (pointIdx < 0)
        return;
    MainWindow::supervisorw->SetupSupervised(pointIdx);
    MainWindow::supervisorw->show();
}

void DatapointView::on_databtn_clicked()
{
    if (pointIdx < 0)
        return;
    xValSupervisor->Setup(pointIdx);
    xValSupervisor->show();
}

void DatapointView::on_labelBtn_clicked()
{
    if (pointIdx < 0)
        return;
    yValSupervisor->Setup(pointIdx, true);
    yValSupervisor->show();
}

