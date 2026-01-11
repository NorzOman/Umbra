
#include "umbra.h"


// -- macros for the icon stuffs --
#define penIconPath    ":/icon/static/pen.png"
#define eraserIconPath ":/icon/static/eraser.png"
#define penX            0
#define penY            32
#define eraserX         12
#define eraserY         29


// -- default constrcutor --
Umbra::Umbra(QWidget *parent) : QWidget(parent){
    // -- Setting up the window --
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::FramelessWindowHint);
    screen = QGuiApplication::primaryScreen();
    rect = screen->geometry();
    setGeometry(rect);

    // -- Setting up the canvas --
    image = QImage(this->size() , QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    setAttribute(Qt::WA_StaticContents);

    // -- Setting up the layout --
    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *watermark = new QLabel("UMBRA");
    watermark->setStyleSheet(
        "font-size: 24px; font-weight: 900; letter-spacing: 7px; color: rgba(255, 255, 255, 80);"
    );
    watermark->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    layout->addWidget(watermark);
    qDebug() << "[debug] added watermark to the widget";
    setLayout(layout);

    // -- Setting up the icons on as default --
    changeIcon();

    // -- setting the color to white as default --
    color = Qt::white;
}


// -- function to change icons depending on the tooltip being used --
void Umbra::changeIcon(){
    // 1 : Denotes the pen icon (default)
    // 2 : Denotes the eraser icon
    if(!hasIcon) return;
    if(type == 1){
        QPixmap pixmap(penIconPath);
        QCursor cursor(pixmap, penX, penY);
        setCursor(cursor);
    } else if(type == 2){
        QPixmap pixmap(eraserIconPath);
        QCursor cursor(pixmap, eraserX, eraserY);
        setCursor(cursor);
    }
}


// -- function to handle key press for the shortcuts --
void Umbra::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape){
        // [esc] to close the widget aka entire application
        this->close();
    }
    else if((event->modifiers() && Qt::ControlModifier) && (event->key() == Qt::Key_C)){
        // [ctrl] + [c] to clear the entire canvas
        image.fill(Qt::transparent);
        update();
    }
    else if((event->modifiers() && Qt::ControlModifier) && (event->key() == Qt::Key_Z && undoStack.size() > 0)){
        // [ctrl] + [z] to pop stack and update;
        image = undoStack.pop();
        update();
    }
}


// -- function handles the paintEvent called on update --
void Umbra::paintEvent(QPaintEvent*){
    QPainter painter(this);
    painter.drawImage(0,0,image);
}


// -- function handles single mouse press actions for dots & eraser reset & type change of toolkit --
void Umbra::mousePressEvent(QMouseEvent *event){
    //-- pushing the last saved image onto update stack --
    undoStack.push(image);
    qDebug() << "[debug] image updated";

    // -- handling for the left click with the type[1] aka pen --
    if(event->button() == Qt::LeftButton && type == 1){
        QPainter painter(&image);
        painter.setPen(QPen(Qt::white , brushSize , Qt::SolidLine, Qt::RoundCap));
        painter.drawPoint(event->pos());
        lastPoint = event->pos();
        update();
    }

    // -- handling reset of the eraser pos on click to prevent line stretch erase problem --
    if(event->button() == Qt::LeftButton && type == 2){
        lastPoint = event->pos();
    }

    // -- handling middle click button for changing the type of tool --
    else if(event->button() & Qt::MiddleButton){
        if(type == 1) type = 2;
        else type = 1;
        changeIcon(); // update the icon according wise
    }
}



void Umbra::mouseMoveEvent(QMouseEvent *event){

    if(event->buttons() & Qt::LeftButton && type == 1){

        // -- line based implementation on pen (much better) --
        QPainter painter(&image);
        painter.setPen(QPen(color , brushSize , Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(lastPoint , event->pos());
        lastPoint = event->pos();
        update();

        // -- ellipse based implementation (wobbly / unstable)==
        /*
        QPainter painter(&image);
        int brushSize = 8;
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(Qt::white , Qt::SolidPattern));
        painter.drawEllipse(lastPoint, brushSize/2, brushSize/2);

        // -- helper function from online that prevents * * * * *  error --
        QPointF p1 = lastPoint;
        QPointF p2 = event->pos();
        float distance = std::hypot(p2.x()-p1.x(), p2.y()-p1.y());
        int steps = std::max(int(distance / (brushSize/2)), 1);
        for(int i=1; i<=steps; ++i){
            QPointF interp = p1 + (p2 - p1) * (float(i)/steps);
            painter.drawEllipse(interp, brushSize/2, brushSize/2);
        }

        lastPoint = event->pos();
        update();
        */
    }

    else if(event->buttons() & Qt::LeftButton && type == 2){

        // -- line based implementaion on eraser --
        changeIcon();
        QPainter painter(&image);
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.setPen(QPen(Qt::transparent , eraserSize , Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(lastPoint , event->pos());
        lastPoint = event->pos();
        update();

        // -- circle absed implmentation --
        /*
        QPainter painter(&image);
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        int brushSize = 16;
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(Qt::transparent , Qt::SolidPattern));
        painter.drawEllipse(lastPoint, brushSize/2, brushSize/2);

        // -- helper function from online that prevents * * * * *  error --
        QPointF p1 = lastPoint;
        QPointF p2 = event->pos();
        float distance = std::hypot(p2.x()-p1.x(), p2.y()-p1.y());
        int steps = std::max(int(distance / (brushSize/2)), 1);
        for(int i=1; i<=steps; ++i){
            QPointF interp = p1 + (p2 - p1) * (float(i)/steps);
            painter.drawEllipse(interp, brushSize/2, brushSize/2);
        }

        lastPoint = event->pos();
        update();
        */
    }
}

// -- function handling the right click menu --
void Umbra::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu(this);

    // -- creating the size slider --
    QSlider* slider = new QSlider(Qt::Horizontal);
    if(type == 1){
        slider->setRange(0,10);
        slider->setValue(brushSize);
    }
    else{
        slider->setRange(10,50);
        slider->setValue(eraserSize);
    }
    slider->setFixedWidth(120);
    QWidgetAction *sliderAction = new QWidgetAction(&menu);
    sliderAction->setDefaultWidget(slider);
    menu.addAction(sliderAction);

    // -- other actions --
    QAction* colorAction = menu.addAction("Pen Color");
    QAction* clearAction = menu.addAction("Clear Canvas");
    QAction* undoAction = menu.addAction("Undo");
    menu.addSeparator();
    QAction* exitAction = menu.addAction("Exit");

    connect(slider, &QSlider::valueChanged, this , [=](int value){
        if(type == 1) brushSize = value;
        else eraserSize = value;
        qDebug() << "[debug] brush size value changed to" << brushSize;
    });

    // -- block state ---
    QAction* selected = menu.exec(event->globalPos());

    // -- handling actions --
    if(selected == clearAction){
        image.fill(Qt::transparent);
        update();
    }

    else if (selected == colorAction) {
        QColor newColor = QColorDialog::getColor(color,this,"Select Pen Color",QColorDialog::ShowAlphaChannel);
        if (newColor.isValid()) {
            color = newColor;
        }
    }

    else if(selected == undoAction){
        if(!undoStack.empty()){
            image = undoStack.pop();
            update();
        }
    }

    else if(selected == exitAction){
        this->close();
    }

}

// EOF :)
