
#pragma once

#include <QWidget>
#include <QScreen>
#include <QRect>
#include <QPoint>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QGuiApplication>
#include <QBrush>
#include <QMenu>

// #include <cmath> <- not needed since no ellipse impleemntation


class Umbra : public QWidget{

    Q_OBJECT

public:
    explicit Umbra(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void contextMenu(QContextMenuEvent* event);

private:
    QImage image;
    QScreen *screen;
    QRect rect;
    QPoint lastPoint;
};
