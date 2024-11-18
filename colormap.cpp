#include "colormap.h"
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QRandomGenerator>

ColorMap::ColorMap(QWidget *parent) : QWidget(parent)
{
    colors = { Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::magenta, Qt::cyan,
               Qt::darkRed, Qt::darkGreen, Qt::darkBlue, Qt::darkYellow, Qt::darkMagenta, Qt::darkCyan };
}

void ColorMap::render()
{
    update();
}

void ColorMap::paintEvent(QPaintEvent* e)
{
    if (image.isNull() || image.width() != width() || image.height() != height())
    {
        image = QImage(width(), height(), QImage::Format_RGB32);
        image.fill(Qt::black);
    }
    QPainter painter(this);
    painter.drawImage(0, 0, image);
}

void ColorMap::setPixel(int x, int y, const QColor& color)
{
    if (x >= 0 && x < image.width() && y >= 0 && y < image.height())
        image.setPixelColor(x, y, color);
}

void ColorMap::setPixel(int x, int y, QVector<double>& encodings)
{
    QColor color = Qt::black;
    for (int i = 0; i < regionNum; i++)
        color = addColors(scaleColor(colors[i], encodings[i]), color);
    setPixel(x, y, color);
}

void ColorMap::setRegionNum(int n)
{
    auto rand = QRandomGenerator::global();
    if (regionNum < n)
        for (int i = regionNum; i < n; i++)
            colors.append(QColor(rand->generate() % 255, rand->generate() % 255, rand->generate() % 255));
    regionNum = n;
}

int ColorMap::getRegionNum() const
{
    return regionNum;
}

QColor ColorMap::scaleColor(const QColor& color, double val)
{
    int red = static_cast<int>(color.red() * val);
    int green = static_cast<int>(color.green() * val);
    int blue = static_cast<int>(color.blue() * val);

    red = qBound(0, red, 255);
    green = qBound(0, green, 255);
    blue = qBound(0, blue, 255);

    return QColor(red, green, blue);
}

QColor ColorMap::addColors(const QColor& a, const QColor& b)
{
    int red = a.red() + b.red();
    int green = a.green() + b.green();
    int blue = a.blue() + b.blue();

    red = qBound(0, red, 255);
    green = qBound(0, green, 255);
    blue = qBound(0, blue, 255);

    return QColor(red, green, blue);
}
