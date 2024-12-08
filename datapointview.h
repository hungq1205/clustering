#ifndef DATAPOINTVIEW_H
#define DATAPOINTVIEW_H

#include <QWidget>
#include "supervisorwindow.h"

namespace Ui {
class DatapointView;
}

class DatapointView : public QWidget
{
    Q_OBJECT

public:
    static SupervisorWindow *yValSupervisor, *xValSupervisor;

    explicit DatapointView(QWidget *parent = nullptr);
    ~DatapointView();
    void SetColor(bool odd);
    void SetPointIndex(int index);
    void UpdateData();
    void UpdateSupervisedData();
    void UpdateLabel();

private slots:
    void on_supervisedbtn_clicked();

    void on_databtn_clicked();

    void on_labelBtn_clicked();

private:
    Ui::DatapointView *ui;
    int pointIdx;

    void onXSupervisorClose(SupervisorWindow caller, const QList<double>& val);
    void onYSupervisorClose(SupervisorWindow caller, const QList<double>& val);
};

#endif // DATAPOINTVIEW_H
