#ifndef COLORMAP_H
#define COLORMAP_H

#include <QWidget>
#include <QImage>
#include <QVector>

class ColorMap : public QWidget
{
    Q_OBJECT
public:
    explicit ColorMap(QWidget *parent = nullptr);
    void render();
    void setPixel(int x, int y, const QColor& color);
    void setPixel(int x, int y, QVector<double>& encodings);
    void setRegionNum(int n);
    int getRegionNum() const;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    static QColor scaleColor(const QColor& color, double val);
    static QColor addColors(const QColor& a, const QColor& b);

    int regionNum;
    QImage image;
    QVector<QColor> colors;
};

#endif // COLORMAP_H
