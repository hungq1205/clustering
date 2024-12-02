#include "supervisorwindow.h"
#include "ui_supervisorwindow.h"
#include "QFocusEvent"
#include "QLabel"
#include "mainwindow.h"

Point* SupervisorWindow::target = nullptr;

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

void SupervisorWindow::InitFields(int clusterNum) {
    this->setFixedHeight(27 * clusterNum + 30);
    fields = QVector<QDoubleSpinBox*>();
    for (int i = 0; i < clusterNum; i++)
        fields.append(createCluterField(QString::fromStdString(std::to_string(i))));
}

QDoubleSpinBox* SupervisorWindow::createCluterField(QString label) {
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

    return input;
}

void SupervisorWindow::showEvent(QShowEvent *e) {
    auto it = MainWindow::supervisedData->find(*target);
    if (it == MainWindow::supervisedData->end()) {
        for (int i = 0; i < fields.size(); i++)
            fields[i]->setValue(0.0);
    }
    else {
        for (int i = 0; i < fields.size(); i++)
            fields[i]->setValue(it.value()[i]);
    }

    QDialog::showEvent(e);
}

void SupervisorWindow::closeEvent(QCloseEvent *e) {
    if (target == nullptr)
        return;

    bool notZero = false;
    auto data = QList<double>(fields.size());
    for (int i = 0; i < fields.size(); i++) {
        data[i] = fields[i]->value();
        if (data[i] > 0.0001)
            notZero = true;
    }

    if (notZero)
        MainWindow::supervisedData->insert(*target, data);
    else
        MainWindow::supervisedData->remove(*target);
}

void SupervisorWindow::focusOutEvent(QFocusEvent *e) {
    close();
    QWidget::focusOutEvent(e);
}
