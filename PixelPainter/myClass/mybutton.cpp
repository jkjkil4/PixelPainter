#include "mybutton.h"

MyButton::MyButton(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    connect(ptimer, SIGNAL(timeout()), this, SLOT(slot_ptimer()));
}

void MyButton::limitSize(QSize size){
    setMaximumSize(size);
    setMinimumSize(size);
}

void MyButton::slot_ptimer(){
    //如果鼠标没在控件内
    if( !QRect(0,0,width(),height()).contains(mapFromGlobal(cursor().pos())) ){
        //如果鼠标被按着
        if( !isMouseHolding ){
            isMouseAt = false;
            update();
        }
        ptimer->stop();
    }
}

void MyButton::mousePressEvent(QMouseEvent *ev){
    //如果按下的是左键
    if( ev->button()==Qt::LeftButton ){
        parentWidget()->parentWidget()->setFocus();
        isMouseHolding = true;
        update();
    }
}
void MyButton::mouseMoveEvent(QMouseEvent *ev){
    //如果有左键
    if( !(ev->buttons() & Qt::LeftButton) ){
        //如果鼠标在控件内
        if( QRect(0,0,width(),height()).contains(ev->pos()) ){
            if( !ptimer->isActive() ){
                isMouseAt = true;
                ptimer->start(delay);
                update();
            }
        }
    }
}
void MyButton::mouseReleaseEvent(QMouseEvent *ev){
    //如果松开的是左键
    if( ev->button()==Qt::LeftButton ){
        //如果鼠标在控件内
        isMouseHolding = false;
        if( QRect(0,0,width(),height()).contains(ev->pos()) ){
            emit clicked();
        }else{
            isMouseAt = false;
        }
        update();
    }
}
void MyButton::paintEvent(QPaintEvent *){
    QPainter p(this);
    QColor col = isEnabled() ? backgroundColor : notEnabledColor;
    if( isMouseHolding ){
        col = QColor( qBound(0, col.red()-offset1, 255), qBound(0, col.green()-offset1, 255), qBound(0, col.blue()-offset1, 255) );
    }else if(isMouseAt){
        col = QColor( qBound(0, col.red()+offset1, 255), qBound(0, col.green()+offset1, 255), qBound(0, col.blue()+offset1, 255) );
    }
    p.fillRect(1, 1, width()-2, height()-2, col);
    p.drawPixmap((width()-pixmap.width())/2, (height()-pixmap.height())/2, pixmap);
    jDrawRecFrame(&p, 0, 0, width(), height(), borderWidth, QColor( qBound(0, col.red()-offset2, 255), qBound(0, col.green()-offset2, 255), qBound(0, col.blue()-offset2, 255) ) );
}
