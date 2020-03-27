#include "jheader.h"

bool isContains(int num1, int num2){
    return ((num1|num2)==num1);
}

void MyAdjustSize(int& w, int& h, int adjustW, int adjustH){
    if( w==0 || h==0 )
        return;
    double kW = static_cast<double>(adjustW)/w;
    double kH = static_cast<double>(adjustH)/h;
    w = kW > kH ? static_cast<int>( w*kH ) : adjustW;
    h = kW > kH ? adjustH : static_cast<int>( h*kW );
}
double lerp(double v1, double v2, double sc){
    return v1 + (v2 - v1)*sc;
}
double myBoundF(double min, double value, double max){
    if( value < min )
        value = min;
    else if( value > max )
        value = max;
    return value;
}
QRgb mixColor(QRgb bjRgba, QRgb qjRgba){
    int bjAlpha = qAlpha(bjRgba);
    int qjAlpha = qAlpha(qjRgba);
    if( qjAlpha == 0 ){
        return bjRgba;
    }else if(qjAlpha == 255){
        return qjRgba;
    }
    int A = 255 - (255 - qjAlpha) * (255 - bjAlpha) / 255;
    if( A == 0 )
        return qRgba(0, 0, 0, 0);
    int R = (qRed(bjRgba)*bjAlpha*(255 - qjAlpha)/255   + qRed(qjRgba)*qjAlpha)   / A;
    int G = (qGreen(bjRgba)*bjAlpha*(255 - qjAlpha)/255 + qGreen(qjRgba)*qjAlpha) / A;
    int B = (qBlue(bjRgba)*bjAlpha*(255 - qjAlpha)/255  + qBlue(qjRgba)*qjAlpha)  / A;
    return qRgba(R, G, B, A);
}

void jDrawText(QPainter *p, QPoint pos, int flags, QString str, int xLimit, int yLimit){jDrawText(p,pos.x(),pos.y(),flags,str,xLimit,yLimit);}
void jDrawText(QPainter *p, int x, int y, int flags, QString str, int xLimit, int yLimit){
    //字体
    QFontMetrics fm(p->font());
    QRect rec = fm.boundingRect(QRect(0,0,INT_MAX,INT_MAX),Qt::TextFlag(),str);
    //大小
    int w=rec.width(),h=rec.height();
    if( xLimit!=-1 && w>xLimit )
        w=xLimit;
    if( yLimit!=-1 && h>yLimit )
        h=yLimit;
    //判断
    x-=w/2;
    y-=h/2;
    if( isContains(flags,Qt::AlignRight) )
        x-=w/2;
    if( isContains(flags,Qt::AlignLeft) )
        x+=w/2;
    if( isContains(flags,Qt::AlignBottom) )
        y-=h/2;
    if( isContains(flags,Qt::AlignTop) )
        y+=h/2;
    //字符串所占的像素宽度,高度
    QRect r=QRect( x,y,w,h );
    //绘制
    //fillRect(r,QColor(255,255,0));  //这是调试用的
    p->drawText(r,flags,str);
}

void jDrawRecFrame(QPainter *p,int x,int y,int w,int h,int width,QColor c){
    //顶部
    p->fillRect(x, y, w, width, c);
    //左侧
    p->fillRect(x, y+width, width, h-width*2, c);
    //右侧
    p->fillRect(x+w-width, y+width, width, h-width*2, c);
    //底部
    p->fillRect(x, y+h-width, w, width, c);
}
