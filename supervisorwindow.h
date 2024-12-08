#ifndef SUPERVISORWINDOW_H
#define SUPERVISORWINDOW_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include "point.h"

namespace Ui {
class SupervisorWindow;
}

class SupervisorWindow : public QDialog
{
    Q_OBJECT

public:
    int writePointIdx = -1;

    explicit SupervisorWindow(QWidget *parent = nullptr);
    ~SupervisorWindow();
    void InitFields(int fieldNum, QStringList lbs, bool readonly=false);
    void SetupSupervised(int pointIdx);
    void Setup(int pointIdx, bool predict=false);

protected:
    void closeEvent(QCloseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    int curFieldNum = 0;
    bool writeSupervised = false;
    bool readonly = false;
    Ui::SupervisorWindow *ui;
    QVector<QDoubleSpinBox*> fields;
    QVector<QLabel*> labels;

    std::tuple<QLabel*, QDoubleSpinBox*> createClusterField(QString label);
};

#endif // SUPERVISORWINDOW_H
