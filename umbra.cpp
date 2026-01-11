
#include "umbra.h"

Umbra::Umbra(QWidget *parent) : QWidget(parent){
    // -- get the size for the primary screen --
    screen = QGuiApplication::primaryScreen();
    rect = screen->geometry();
    // -- set the geometry of the entire canvas as the full screen
    setGeometry(rect);

    // -- create the canvas as big as the widget
    image = QImage(this->size() , QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    setAttribute(Qt::WA_StaticContents);
}

void Umbra::paintEvent(QPaintEvent*){
    QPainter painter(this);
    painter.drawImage(0,0,image);
}

void Umbra::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton)
        lastPoint = event->pos();
}

void Umbra::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape){
        this->close();
    }
}

void Umbra::mouseMoveEvent(QMouseEvent *event){
    // -- buttons cause its move event
    if(event->buttons() & Qt::LeftButton){
        QPainter painter(&image);
        painter.setPen(QPen(Qt::black , 3 , Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(lastPoint , event->pos());
        lastPoint = event->pos();
        update();
    }
}
