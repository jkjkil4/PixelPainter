#include "jheader.h"

bool isContains(int num1, int num2){
    return ((num1|num2)==num1);
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
void jDrawRecFrame(QPainter *p,QRect rect,int width,QColor c){jDrawRecFrame(p,rect.x(),rect.y(),rect.width(),rect.height(),width,c);}
void jDrawRecFrame(QPainter *p,int x,int y,int w,int h,int width,QColor c){
    p->fillRect(x,y,w,width,c);
    p->fillRect(x,y,width,h,c);
    p->fillRect(x+w-width,y,width,h,c);
    p->fillRect(x,y+h-width,w,width,c);
}
