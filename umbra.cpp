
#include "umbra.h"

Umbra::Umbra(QWidget *parent) : QWidget(parent){
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::FramelessWindowHint);
    // -- get the size for the primary screen --
    screen = QGuiApplication::primaryScreen();
    rect = screen->geometry();
    // -- set the geometry of the entire canvas as the full screen
    setGeometry(rect);

    // -- create the canvas as big as the widget
    image = QImage(this->size() , QImage::Format_ARGB32_Premultiplied);
    // image.fill(Qt::white);
    image.fill(Qt::transparent);

    setAttribute(Qt::WA_StaticContents);
}

void Umbra::paintEvent(QPaintEvent*){
    QPainter painter(this);
    painter.drawImage(0,0,image);
}

void Umbra::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        QPainter painter(&image);
        painter.setPen(QPen(Qt::white , 3 , Qt::SolidLine, Qt::RoundCap));
        painter.drawPoint(event->pos());
        lastPoint = event->pos();
        update();
    }

    // -- condition called when middle button for erase --
    // -- resets the pos to prevent line erasing for no reason --
    // -- between the old end and the new start --
    if(event->button() == Qt::MiddleButton){
        lastPoint = event->pos();
    }
}

void Umbra::keyPressEvent(QKeyEvent *event){

    if(event->key() == Qt::Key_Escape){
        this->close();
    }

    else if((event->modifiers() && Qt::ControlModifier) && (event->key() == Qt::Key_C)){
        image.fill(Qt::transparent);
        update();
    }
}

void Umbra::mouseMoveEvent(QMouseEvent *event){
    // -- buttons cause its move event
    if(event->buttons() & Qt::LeftButton){
        // -- line based implementation (much better) --
        QPainter painter(&image);
        painter.setPen(QPen(Qt::white , 8 , Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(lastPoint , event->pos());
        lastPoint = event->pos();
        update();

        // -- ellipse based implementation ==
        // QPainter painter(&image);
        // int brushSize = 8;
        // painter.setPen(Qt::NoPen);
        // painter.setBrush(QBrush(Qt::white , Qt::SolidPattern));
        // painter.drawEllipse(lastPoint, brushSize/2, brushSize/2);

        // // -- helper function from online that prevents * * * * *  error --
        // QPointF p1 = lastPoint;
        // QPointF p2 = event->pos();
        // float distance = std::hypot(p2.x()-p1.x(), p2.y()-p1.y());
        // int steps = std::max(int(distance / (brushSize/2)), 1);
        // for(int i=1; i<=steps; ++i){
        //     QPointF interp = p1 + (p2 - p1) * (float(i)/steps);
        //     painter.drawEllipse(interp, brushSize/2, brushSize/2);
        // }

        // lastPoint = event->pos();
        // update();
    }

    else if(event->buttons() & Qt::MiddleButton){

        // -- line based implementaion --
        QPainter painter(&image);
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.setPen(QPen(Qt::transparent , 8 , Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(lastPoint , event->pos());
        lastPoint = event->pos();
        update();

        // -- circle absed implmentation --
        // QPainter painter(&image);
        // painter.setCompositionMode(QPainter::CompositionMode_Clear);
        // int brushSize = 16;
        // painter.setPen(Qt::NoPen);
        // painter.setBrush(QBrush(Qt::transparent , Qt::SolidPattern));
        // painter.drawEllipse(lastPoint, brushSize/2, brushSize/2);

        // // -- helper function from online that prevents * * * * *  error --
        // QPointF p1 = lastPoint;
        // QPointF p2 = event->pos();
        // float distance = std::hypot(p2.x()-p1.x(), p2.y()-p1.y());
        // int steps = std::max(int(distance / (brushSize/2)), 1);
        // for(int i=1; i<=steps; ++i){
        //     QPointF interp = p1 + (p2 - p1) * (float(i)/steps);
        //     painter.drawEllipse(interp, brushSize/2, brushSize/2);
        // }

        // lastPoint = event->pos();
        // update();
    }

}






