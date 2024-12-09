#include "supervisorwindow.h"
#include "ui_supervisorwindow.h"
#include "QFocusEvent"
#include "QLabel"
#include "mainwindow.h"

SupervisorWindow::SupervisorWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SupervisorWindow)
{
    ui->setupUi(this);
}

SupervisorWindow::~SupervisorWindow()
{
    delete ui;
}

void SupervisorWindow::InitFields(int fieldNum, QStringList lbs, bool readonly) {
    this->setFixedHeight(27 * fieldNum + 30);
    this->readonly = readonly;
    if (curFieldNum == 0)
    {
        fields = QVector<QDoubleSpinBox*>();
        labels = QVector<QLabel*>();
    }
    for (int i = curFieldNum; i < fieldNum; i++)
    {
        auto t = createClusterField(lbs[i]);
        labels.append(std::get<0>(t));
        fields.append(std::get<1>(t));
        fields[i]->setEnabled(!readonly);
    }
    for (int i = fieldNum; i < curFieldNum; i++)
    {
        labels[i]->deleteLater();
        fields[i]->deleteLater();
    }
    curFieldNum = fieldNum;
}

void SupervisorWindow::Setup(int pointIdx, bool predict) {
    if (pointIdx < 0)
        return;

    if (predict) {
        auto pred = MainWindow::cmethod->predict(*(*MainWindow::inferPoints)[pointIdx]);
        for (int i = 0; i < fields.size(); i++)
            fields[i]->setValue(pred[i]);
    }
    else {
        for (int i = 0; i < fields.size(); i++)
            fields[i]->setValue((*MainWindow::points)[pointIdx]->x[i]);
    }
    writeSupervised = false;
    writePointIdx = pointIdx;
}

void SupervisorWindow::SetupSupervised(int pointIdx) {
    if (pointIdx < 0)
        return;

    auto p = (*MainWindow::points)[pointIdx];
    auto it = (*MainWindow::supervisedData).find(*p);
    if (it == (*MainWindow::supervisedData).end()) {
        for (int i = 0; i < fields.size(); i++)
            fields[i]->setValue(0.0);
    }
    else {
        for (int i = 0; i < fields.size(); i++)
            fields[i]->setValue(it.value()[i]);
    }
    writeSupervised = true;
    writePointIdx = pointIdx;
}

std::tuple<QLabel*, QDoubleSpinBox*> SupervisorWindow::createClusterField(QString label) {
    QLabel *lb = new QLabel();
    lb->setText(label);
    lb->setAlignment(Qt::AlignCenter);
    lb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QDoubleSpinBox *input = new QDoubleSpinBox();
    input->setSingleStep(0.1);
    input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(lb);
    hlayout->addWidget(input);
    hlayout->setStretch(0, 2);
    hlayout->setStretch(1, 8);

    ui->layout->addLayout(hlayout);

    return std::make_tuple(lb, input);
}

void SupervisorWindow::closeEvent(QCloseEvent *e) {
    if (readonly || writePointIdx < 0)
        return;

    if (writeSupervised) {
        bool notZero = false;
        auto data = new QList<double>();
        for (int i = 0; i < fields.size(); i++) {
            (*data).append(fields[i]->value());
            if ((*data)[i] > 0.0001)
                notZero = true;
        }

        if (notZero)
            MainWindow::supervisedData->insert(*(*MainWindow::points)[writePointIdx], *data);
        else
        {
            delete data;
            MainWindow::supervisedData->remove(*(*MainWindow::points)[writePointIdx]);
        }
        if (MainWindow::pointViews != nullptr && MainWindow::pointViews->size() > writePointIdx)
            (*MainWindow::pointViews)[writePointIdx]->UpdateSupervisedData();
    }
    else {
        QList<double> vals;
        for (int i = 0; i < fields.size(); i++)
            vals.append(fields[i]->value());
        MainWindow::writePoint(writePointIdx, vals);
        if (MainWindow::pointViews != nullptr && MainWindow::pointViews->size() > writePointIdx)
            (*MainWindow::pointViews)[writePointIdx]->UpdateData();
    }
}

void SupervisorWindow::focusOutEvent(QFocusEvent *e) {
    close();
    QWidget::focusOutEvent(e);
}
