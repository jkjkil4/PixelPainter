#include "myline.h"

MyLine::MyLine(int type, QWidget *parent) : QWidget(parent)
{
    this->type = type;
    updateState();
}

void MyLine::updateState(){
    switch(type){
    case Qt::Horizontal:
        setMaximumHeight(w);
        setMinimumHeight(w);
        break;
    case Qt::Vertical:
        setMaximumWidth(w);
        setMinimumWidth(w);
        break;
    }
    update();
}
void MyLine::setWidth(int w){
    this->w = w;
    updateState();
}
void MyLine::setColor(QColor c){
    color = c;
    update();
}

void MyLine::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.fillRect(0, 0, width(), height(), color);
}
