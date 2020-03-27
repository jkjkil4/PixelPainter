#include "mynumscrollbar.h"

MyNumScrollBar::MyNumScrollBar(QWidget *parent) : QWidget(parent){
    limit->setSingleShot(true);
    connect(limit, &QTimer::timeout, [=](){
        emit valueChanged();
        update();
    });
}

void MyNumScrollBar::updateValue(int mouseX){
    int value = (max-min) * qBound( 0, mouseX-1, width()-2 ) / (width()-2) + min;
    if( value!=this->value ){
        this->value = value;
        if( !limit->isActive() )
            limit->start(delay);
    }
}

void MyNumScrollBar::mousePressEvent(QMouseEvent *ev){
    parentWidget()->parentWidget()->setFocus();
    updateValue(ev->x());
}
void MyNumScrollBar::mouseMoveEvent(QMouseEvent *ev){
    parentWidget()->parentWidget()->setFocus();
    updateValue(ev->x());
}
void MyNumScrollBar::paintEvent(QPaintEvent *){
    QPainter p(this);

    int w = (width()-2) * value / (max-min) + 1;
    p.fillRect(1, 1, w, height()-2, isEnabled() ? QColor(180, 180, 180) : QColor(220, 220, 220));
    p.fillRect(1+w, 1, width()-2-w, height()-2, QColor(240, 240, 240));

    jDrawText(&p, width()-4, height()/2, Qt::AlignRight, QString::number(value));

    jDrawRecFrame(&p, 0, 0, width(), height(), 1, QColor(80,80,80));
}
