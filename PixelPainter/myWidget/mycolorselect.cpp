#include "mycolorselect.h"

//色相选择-----------------------
MyColorSelect::HueSelect::HueSelect(MyVars *vars, QWidget *updateWidget, QTimer *fakeSlot){
    this->vars = vars;
    this->updateWidget = updateWidget;
    this->fakeSlot = fakeSlot;
    setMaximumSize(25,140);
    setMinimumSize(25,140);
}
void MyColorSelect::HueSelect::updateHue(int mouseY){
    int y = qBound(0, mouseY, height()-2);
    vars->color.h = 360*y/(height()-2);
    QColor col;
    col.setHsv(vars->color.h, 255, 255);
    vars->color.solidR = col.red();
    vars->color.solidG = col.green();
    vars->color.solidB = col.blue();
    update();
    fakeSlot->start(0);
}
void MyColorSelect::HueSelect::mousePressEvent(QMouseEvent *ev){
    parentWidget()->parentWidget()->setFocus();
    updateHue(ev->y());
}
void MyColorSelect::HueSelect::mouseMoveEvent(QMouseEvent *ev){
    parentWidget()->parentWidget()->setFocus();
    updateHue(ev->y());
}
void MyColorSelect::HueSelect::paintEvent(QPaintEvent *){
    QPainter p(this);

    QColor hsv;
    int hMax = height()-2;
    for( int i = 0; i<hMax; i++ ){
        hsv.setHsv(360*i/hMax, 255, 255);
        p.fillRect(1, 1+i, width()-2, 1, hsv);
    }

    QColor colRgb;
    colRgb.setHsv(vars->color.h, 255, 255);
    QColor reColor = QColor( 255-colRgb.red(), 255-colRgb.green(), 255-colRgb.blue() );
    p.fillRect(1, hMax*vars->color.h/360-1, width()-2, 4, reColor);

    jDrawRecFrame(&p, 0, 0, width(), height(), 1, QColor(0,0,0));
}


//饱和度、明度选择
MyColorSelect::SVSelect::SVSelect(MyVars *vars, QTimer *fakeSlot){
    this->vars = vars;
    this->fakeSlot = fakeSlot;
    setMaximumSize(140,140);
    setMinimumSize(140,140);
    posNow = QPoint(width()-2, 0);
}
void MyColorSelect::SVSelect::updateSV(QPoint pos, bool updateAll = false){
    posNow = QPoint( qBound(0, pos.x(), width()-2), qBound(0, pos.y(), height()-2) );
    //得到颜色
    QColor col;
    int s = 255*posNow.x()/(width()-2),v = 255 - 255*posNow.y()/(height()-2);
    col.setHsv( vars->color.h, s, v );
    vars->color.r = col.red();
    vars->color.g = col.green();
    vars->color.b = col.blue();
    vars->color.s = s;
    vars->color.v = v;
    //update
    if(updateAll){
        update();
    }else{
        limitRect = true;
        update(QRegion(posNow.x()-w, posNow.y()-w, 2*w, 2*w)+QRegion(posBefore.x()-w, posBefore.y()-w, 2*w, 2*w));
    }
}
void MyColorSelect::SVSelect::mousePressEvent(QMouseEvent *ev){
    parentWidget()->parentWidget()->setFocus();
    updateSV(ev->pos());
    fakeSlot->start(0);
}
void MyColorSelect::SVSelect::mouseMoveEvent(QMouseEvent *ev){
    parentWidget()->parentWidget()->setFocus();
    updateSV(ev->pos());
    fakeSlot->start(0);
}
void MyColorSelect::SVSelect::paintEvent(QPaintEvent *){
    QPoint temp = posNow;
    QPainter p(this);

    MyColor color = vars->color;
    int rSub = 255 - color.solidR, gSub = 255 - color.solidG, bSub = 255 - color.solidB;
    for( int i=0; i<width()-2; i++ ){
        if(limitRect && (i<posBefore.x()-w-1 || i>posBefore.x()+w) && (i<temp.x()-w-1 || i>temp.x()+w))
            continue;
        int iR = 255 - rSub*i/(width()-3);
        int iG = 255 - gSub*i/(width()-3);
        int iB = 255 - bSub*i/(width()-3);
        for( int j=0; j<height()-2; j++ ){
            if(limitRect && (j<posBefore.y()-w-1 || j>posBefore.y()+w) && (j<temp.y()-w-1 || j>temp.y()+w))
                continue;
            int r = iR * (height() - j)/height();
            int g = iG * (height() - j)/height();
            int b = iB * (height() - j)/height();
            p.fillRect( 1+i, 1+j, 1, 1, QColor(r,g,b) );
        }
    }
    jDrawRecFrame(&p, 0, 0, width(), height(), 1, QColor(0,0,0));
    QColor col;
    col.setHsv( vars->color.h, 255*temp.x()/(width()-2), 255 - 255*temp.y()/(height()-2) );
    p.setPen(QPen(QColor(255-col.red(), 255-col.green(), 255-col.blue()), 3));
    p.drawEllipse(temp+QPoint(1,1), w-2, w-2);

    posBefore = temp;
    limitRect = false;
}


//颜色选择---------(上面两个类是该类的嵌套类)--------------
MyColorSelect::MyColorSelect(MyVars *vars, QWidget *parent) : QGroupBox(parent){
    this->vars = vars;
    setTitle("颜色选择");

    //布局
    svSelect = new SVSelect(vars, fakeSlotSV);
    hueSelect = new HueSelect(vars, svSelect, fakeSlotHue);
    labels[0] = new QLabel("R");
    labels[1] = new QLabel("G");
    labels[2] = new QLabel("B");
    labels[3] = new QLabel("H");
    labels[4] = new QLabel("S");
    labels[5] = new QLabel("V");
    for( int i=0; i<6; i++ ){
        spinBoxes[i] = new MySpinBox;
        spinBoxes[i]->setMaximumWidth(50);
        spinBoxes[i]->mySetMax(i!=3 ? 255 : 360);
        labels[i]->setMaximumWidth(8);
        //绑定信号与槽
        if( i<3 ){
            connect(spinBoxes[i], SIGNAL(valueChanged(int)), this, SLOT(slot_onRGBChanged()));
        }else{
            connect(spinBoxes[i], SIGNAL(valueChanged(int)), this, SLOT(slot_onHSVChanged()));
        }
    }
    spinBoxes[0]->setStyleSheet("background-color:rgb(255,200,200);");
    spinBoxes[1]->setStyleSheet("background-color:rgb(200,255,200);");
    spinBoxes[2]->setStyleSheet("background-color:rgb(200,200,255);");

    QHBoxLayout* lay1 = new QHBoxLayout;
    lay1->setMargin(3);
    lay1->setSpacing(2);
    lay1->addWidget(hueSelect);
    lay1->addWidget(svSelect);

    QVBoxLayout* layLeft = new QVBoxLayout;
    QVBoxLayout* layRight = new QVBoxLayout;
    for(int i=0; i<3; i++){
        QHBoxLayout* lay1 = new QHBoxLayout;
        lay1->addWidget(labels[i]);
        lay1->addWidget(spinBoxes[i]);
        layLeft->addLayout(lay1);
        QHBoxLayout* lay2 = new QHBoxLayout;
        lay2->addWidget(labels[3+i]);
        lay2->addWidget(spinBoxes[3+i]);
        layRight->addLayout(lay2);
    }

    QHBoxLayout* lay2 = new QHBoxLayout;
    lay2->addLayout(layLeft);
    lay2->addLayout(layRight);

    QVBoxLayout* layMain = new QVBoxLayout;
    layMain->setMargin(4);
    layMain->setSpacing(2);
    layMain->addLayout(lay1);
    layMain->addLayout(lay2);

    setLayout(layMain);

    //假槽
    fakeSlotHue->setSingleShot(true);
    fakeSlotSV->setSingleShot(true);
    connect(fakeSlotHue, &QTimer::timeout, [=](){
        needUpdateHue = true;
        if( !limitRateHue->isActive() )
            limitRateHue->start(20);
    });
    fakeSlotSV->setSingleShot(true);
    connect(fakeSlotSV, &QTimer::timeout, [=](){
        needUpdateSV = true;
        if( !limitRateSV->isActive() )
            limitRateSV->start(20);
    });
    connect(limitRateHue, &QTimer::timeout, [=](){
        if( needUpdateHue == true ){
            svSelect->updateSV(svSelect->posNow, true);
            updateSpinBoxes();
            needUpdateHue = false;
            emit colorChanged();
        }else{
            limitRateHue->stop();
        }
    });
    connect(limitRateSV, &QTimer::timeout, [=](){
        if( needUpdateSV == true ){
            updateSpinBoxes();
            needUpdateSV = false;
            emit colorChanged();
        }else{
            limitRateSV->stop();
        }
    });

    updateSpinBoxes();
}
void MyColorSelect::updateSpinBoxes(){
    MyColor col = vars->color;
    spinBoxes[0]->mySetValue(col.r);
    spinBoxes[1]->mySetValue(col.g);
    spinBoxes[2]->mySetValue(col.b);
    spinBoxes[3]->mySetValue(col.h);
    spinBoxes[4]->mySetValue(col.s);
    spinBoxes[5]->mySetValue(col.v);
}
void MyColorSelect::slot_onRGBChanged(){
    int r = qBound(0, spinBoxes[0]->value(), 255);
    int g = qBound(0, spinBoxes[1]->value(), 255);
    int b = qBound(0, spinBoxes[2]->value(), 255);
    QColor col(r,g,b);
    int h = qBound(0, col.hue(), 360);
    int s = col.saturation();
    int v = col.value();
    QColor reColor;
    reColor.setHsv(h, 255, 255);
    //设置颜色
    vars->color.r = r;
    vars->color.g = g;
    vars->color.b = b;
    vars->color.h = h;
    vars->color.s = s;
    vars->color.v = v;
    vars->color.solidR = reColor.red();
    vars->color.solidG = reColor.green();
    vars->color.solidB = reColor.blue();
    //设置HSV的SpinBox
    spinBoxes[3]->mySetValue(h);
    spinBoxes[4]->mySetValue(s);
    spinBoxes[5]->mySetValue(v);
    //坐标
    svSelect->posNow = QPoint((svSelect->width()-2)*s/255, (svSelect->height()-2)*(255-v)/255);
    //update
    update();

    emit colorChanged();
}
void MyColorSelect::slot_onHSVChanged(){
    int h = qBound(0, spinBoxes[3]->value(), 360);
    int s = qBound(0, spinBoxes[4]->value(), 255);
    int v = qBound(0, spinBoxes[5]->value(), 255);
    QColor col;
    col.setHsv(h, s, v);
    int r = col.red();
    int g = col.green();
    int b = col.blue();
    QColor reColor;
    reColor.setHsv(h, 255, 255);
    //设置颜色
    vars->color.h = h;
    vars->color.s = s;
    vars->color.v = v;
    vars->color.r = r;
    vars->color.g = g;
    vars->color.b = b;
    vars->color.solidR = reColor.red();
    vars->color.solidG = reColor.green();
    vars->color.solidB = reColor.blue();
    //设置RGB的SpinBox
    spinBoxes[0]->mySetValue(r);
    spinBoxes[1]->mySetValue(g);
    spinBoxes[2]->mySetValue(b);
    //坐标
    svSelect->posNow = QPoint((svSelect->width()-2)*s/255, (svSelect->height()-2)*(255-v)/255);
    //Update
    update();

    emit colorChanged();
}



