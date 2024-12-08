#ifndef COLORMAP_H
#define COLORMAP_H

#include "point.h"
#include <QWidget>
#include <QImage>
#include <QVector>

class ColorMap : public QWidget
{
    Q_OBJECT
public:
    static bool isSupervising;
    int pointRad = 6;

    explicit ColorMap(QWidget *parent = nullptr);
    void render(QVector<Point*> *points = nullptr);
    void setPixel(int x, int y, const QColor& color);
    void setPixel(int x, int y, QList<double>& encodings);
    void setRegionNum(int n);
    int getRegionNum() const;

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    static QColor scaleColor(const QColor& color, double val);
    static QColor addColors(const QColor& a, const QColor& b);
    QColor colorOf(QList<double>& enc, bool whitePadding=true);

    QVector<QColor> colors;
    int regionNum;
    QImage image;
};

#endif // COLORMAP_H
