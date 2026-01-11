
#pragma once

#include <QWidget>
#include <QScreen>
#include <QDebug>
#include <QRect>
#include <QPoint>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QGuiApplication>
#include <QBrush>
#include <QMenu>
#include <QAction>
#include <QStack>
#include <QSlider>
#include <QWidgetAction>
#include <QContextMenuEvent>


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
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QImage image;
    QScreen *screen;
    QRect rect;
    QPoint lastPoint;
    QStack<QImage> undoStack;
    int brushSize = 5;
};
