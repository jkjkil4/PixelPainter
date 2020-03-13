#include "myalphaselect.h"

//--花里胡哨的透明度选择框---------
MyAlphaSelect::MyAlphaView::MyAlphaView(MyVars* vars, MySpinBox* spinBox){
    this->vars = vars;
    this->spinBox = spinBox;
}
void MyAlphaSelect::MyAlphaView::updateAlpha(int mouseX){
    xNow = qBound(0, mouseX-1, width()-2);
    int alpha = 255 - 255*xNow/(width()-2);
    vars->color.alpha = alpha;
    spinBox->mySetValue(alpha);
    limitRect = true;
    update(QRegion(xNow-1, 1, 4, height()-2) + QRegion(xBefore-1, 1, 4, height()-2));
}
void MyAlphaSelect::MyAlphaView::mousePressEvent(QMouseEvent *ev){
    parentWidget()->parentWidget()->setFocus();
    updateAlpha(ev->x());
}
void MyAlphaSelect::MyAlphaView::mouseMoveEvent(QMouseEvent *ev){
    parentWidget()->parentWidget()->setFocus();
    updateAlpha(ev->x());
}
void MyAlphaSelect::MyAlphaView::paintEvent(QPaintEvent *){
    int temp = xNow;
    QPainter p(this);

    int w = 8;
    int xMax = (width()-2)/w + 1, yMax = (height()-2)/w + 1;
    //绘制背景
    for( int j=0; j<yMax; j++ ){
        bool jFlag = j%2;
        for( int i=0; i<xMax; i++ ){
            bool iFlag = i%2;
            int gray = (iFlag==jFlag) ? 204 : 255 ;
            p.fillRect(1 + i*w, 1 + j*w, w, w, QColor(gray, gray, gray));
        }
    }
    //绘制颜色
    MyColor col = vars->color;
    for( int i=0; i<width()-2; i++ ){
        if( limitRect && (i<xBefore-1 && i>xBefore+3) )
            continue;
        p.setPen(QColor(col.r, col.g, col.b, 255 - 255*i/(width()-3)));
        p.drawRect(i+1, 1, 1, height()-2);
    }
    //绘制竖线()
    int a = 255 - 255*(temp - 1)/(width()-3);
    int R = ( col.r*a + 230*(255-a) )/255;
    int G = ( col.g*a + 230*(255-a) )/255;
    int B = ( col.b*a + 230*(255-a) )/255;
    p.fillRect( temp-1, 1, 4, height()-2, QColor(255-R,255-G,255-B) );

    jDrawRecFrame(&p, 0, 0, width(), height(), 1, QColor(0,0,0));

    xBefore = xNow;
    limitRect = false;
}



//----------上面的那个类是该类的嵌套类------
MyAlphaSelect::MyAlphaSelect(MyVars *vars, QWidget *parent) : QGroupBox(parent){
    this->vars = vars;
    spinBoxAlpha->mySetMax(255);
    spinBoxAlpha->mySetValue(255);
    view = new MyAlphaView(vars, spinBoxAlpha);
    view->setMinimumHeight(18);
    view->setMaximumHeight(18);
    setTitle("透明度");

    //布局
    QHBoxLayout* lay1 = new QHBoxLayout;
    lay1->setAlignment(Qt::AlignLeft);
    lay1->addWidget(labelAlpha);
    lay1->addWidget(spinBoxAlpha);

    QVBoxLayout* layMain = new QVBoxLayout;
    layMain->setMargin(4);
    layMain->setSpacing(2);
    layMain->addLayout(lay1);
    layMain->addWidget(view);

    setLayout(layMain);

}
void MyAlphaSelect::otherColorChanged(){
    view->update();
}
