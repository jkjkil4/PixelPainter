#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{
    ptimer->setSingleShot(true);
    connect(ptimer, &QTimer::timeout, [=](){
        setUpdatesEnabled(true);
        repaint();
    });
}

void MyWidget::resizeEvent(QResizeEvent *){
    setUpdatesEnabled(false);
    if( !ptimer->isActive() )
        ptimer->start(delay);
}
