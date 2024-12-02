#ifndef SUPERVISORWINDOW_H
#define SUPERVISORWINDOW_H

#include <QDialog>
#include <QDoubleSpinBox>
#include "point.h"

namespace Ui {
class SupervisorWindow;
}

class SupervisorWindow : public QDialog
{
    Q_OBJECT

public:
    static Point* target;

    explicit SupervisorWindow(QWidget *parent = nullptr);
    ~SupervisorWindow();
    void InitFields(int clusterNum);

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    Ui::SupervisorWindow *ui;
    QVector<QDoubleSpinBox*> fields;

    QDoubleSpinBox* createCluterField(QString label);
};

#endif // SUPERVISORWINDOW_H
