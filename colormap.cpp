#include "colormap.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QMouseEvent>
#include "QDebug"
#include "mainwindow.h"
#include "clustermethod.h"

bool ColorMap::isSupervising = false;

ColorMap::ColorMap(QWidget *parent) : QWidget(parent) {
    colors = { Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::magenta, Qt::cyan,
               Qt::darkRed, Qt::darkGreen, Qt::darkBlue, Qt::darkYellow, Qt::darkMagenta, Qt::darkCyan };
}

void ColorMap::render(QVector<Point*> *points) {
    if (points != nullptr && MainWindow::points != points) {
        delete MainWindow::points;
        MainWindow::points = points;
    }
    update();
}

void ColorMap::mouseReleaseEvent(QMouseEvent* e) {
    if (isSupervising) return;

    MainWindow::points->append(new Point(e->pos().x(), e->pos().y()));
    MainWindow::inferPoints->append(new Point(e->pos().x(), e->pos().y()));
    render();
}

void ColorMap::mouseDoubleClickEvent(QMouseEvent* e) {
    if (!isSupervising) return;

    Point mpos = Point(e->pos().x(), e->pos().y());
    double dmin = pointRad + 1;
    int min = 0;
    for (int i = 1; i < MainWindow::points->size(); i++) {
        double d = ClusterMethod::euclideanDistance(*((*MainWindow::points)[i]), mpos);
        if (d <= pointRad && d < dmin) {
            dmin = d;
            min = i;
        }
    }
    MainWindow::supervisorw->SetupSupervised(min);
    MainWindow::supervisorw->show();
    render();
}

void ColorMap::paintEvent(QPaintEvent* e) {
    if (image.isNull() || image.width() != width() || image.height() != height()) {
        image = QImage(width(), height(), QImage::Format_RGB32);
        image.fill(Qt::black);
    }
    QPainter painter(this);
    painter.drawImage(0, 0, image);

    QPen pen;
    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::white));
    for (auto p : *MainWindow::points) {
        auto it = MainWindow::supervisedData->find(*p);
        if (it == MainWindow::supervisedData->end())
            painter.drawEllipse(QPoint(p->x[MainWindow::xfield], p->x[MainWindow::yfield]), pointRad, pointRad);
        else {
            QColor color = colorOf(it.value());
            pen.setColor(color);
            painter.setPen(pen);
            painter.setBrush(QBrush(color));
            painter.drawEllipse(QPoint(p->x[MainWindow::xfield], p->x[MainWindow::yfield]), pointRad, pointRad);
            pen.setColor(Qt::white);
            painter.setPen(pen);
            painter.setBrush(QBrush(Qt::white));
        }
    }
}

void ColorMap::setPixel(int x, int y, const QColor& color) {
    if (x >= 0 && x < image.width() && y >= 0 && y < image.height())
        image.setPixelColor(x, y, color);
}

void ColorMap::setPixel(int x, int y, QList<double>& encodings) {
    setPixel(x, y, colorOf(encodings, false));
}

QColor ColorMap::colorOf(QList<double>& enc, bool whitePadding) {
    QColor color = Qt::black;
    if (whitePadding) {
        double supplement = 1.0;
        for (auto it : enc)
            supplement -= it;
        color = scaleColor(Qt::white, supplement);
    }

    for (int i = 0; i < regionNum; i++)
        color = addColors(scaleColor(colors[i], enc[i]), color);
    return color;
}

void ColorMap::setRegionNum(int n) {
    auto rand = QRandomGenerator::global();
    if (regionNum < n)
        for (int i = regionNum; i < n; i++)
            colors.append(QColor(rand->generate() % 255, rand->generate() % 255, rand->generate() % 255));
    regionNum = n;
}

int ColorMap::getRegionNum() const {
    return regionNum;
}

QColor ColorMap::scaleColor(const QColor& color, double val) {
    int red = static_cast<int>(color.red() * val);
    int green = static_cast<int>(color.green() * val);
    int blue = static_cast<int>(color.blue() * val);

    red = qBound(0, red, 255);
    green = qBound(0, green, 255);
    blue = qBound(0, blue, 255);

    return QColor(red, green, blue);
}

QColor ColorMap::addColors(const QColor& a, const QColor& b) {
    int red = a.red() + b.red();
    int green = a.green() + b.green();
    int blue = a.blue() + b.blue();

    red = qBound(0, red, 255);
    green = qBound(0, green, 255);
    blue = qBound(0, blue, 255);

    return QColor(red, green, blue);
}
