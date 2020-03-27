#include "myguideview.h"

//显示区域-------
using namespace _MyGuideView;
MyImageView::MyImageView(MyVars *vars, QWidget *parent) : QWidget(parent){
    this->vars = vars;
}
void MyImageView::mousePressEvent(QMouseEvent *ev){

}
void MyImageView::mouseMoveEvent(QMouseEvent *ev){

}
void MyImageView::paintEvent(QPaintEvent *){
    QPainter p(this);

    jDrawRecFrame(&p, 0, 0, width(), height(), 1, Qt::darkGray);
}



//----导航器------------------
MyGuideView::MyGuideView(MyVars* vars, QWidget *parent) : QGroupBox(parent){
    this->vars = vars;
    setTitle("导航器");
    setMaximumSize(210, 100);
    setMinimumSize(210, 100);

    view = new _MyGuideView::MyImageView(vars);

    btnSmaller->limitSize(26,16);
    btnSmaller->pixmap = QPixmap(":/guide/pic/small.png");
    btnBigger->limitSize(26,16);
    btnBigger->pixmap = QPixmap(":/guide/pic/big.png");
    slider->setMaximumSize(90, 16);
    slider->setMinimumSize(90, 16);
    slider->setMinimum(5);
    slider->setMaximum(1600);
    slider->setValue(100);
    connect(slider, &QSlider::valueChanged, [=](){
        lab->setText( QString::number(slider->value())+"%" );
    });
    connect(btnSmaller, &MyButton::clicked, [=](){
        int num = slider->value();
        int res = 100;
        if( num>100 ){
            int sur = num%100;
            res = sur ? num - sur : num - 100;
            if( num>600 )
                res -= 100;
        }else if( num<10 ){
            res = slider->minimum();
        }else{
            res = num*2/3;
        }
        slider->setValue( qBound(slider->minimum(), res, slider->maximum()) );
    });
    connect(btnBigger, &MyButton::clicked, [=](){
        int num = slider->value();
        int res = 100;
        if( num<100 ){
            res = num*5/3;
            if( res > 100 )
                res = 100;
        }else{
            int sur = num%100;
            res = sur ? num - sur + 100 : num + 100;
            if( num > 600 )
                res += 100;
        }
        slider->setValue( qBound(slider->minimum(), res, slider->maximum()) );
    });
    lab->setMaximumSize(30, 16);
    lab->setMinimumSize(30, 16);

    QHBoxLayout *layBottom = new QHBoxLayout;
    layBottom->setSpacing(1);
    layBottom->setMargin(1);
    layBottom->addWidget(lab);
    layBottom->addWidget(btnSmaller);
    layBottom->addWidget(slider);
    layBottom->addWidget(btnBigger);

    QVBoxLayout *layMain = new QVBoxLayout;
    layMain->setMargin(4);
    layMain->setSpacing(3);
    layMain->addWidget(view);
    layMain->setStretch(0, 1);
    layMain->addLayout(layBottom);

    setLayout(layMain);
}
void MyGuideView::mainImageChanged(){
    slider->setValue(100);
}
