#ifndef JHEADER_H
#define JHEADER_H

#include <QDebug>
#include <QPainter>
//#include <QDateTime>

#define qd qDebug()

bool isContains(int num1, int num2);

void MyAdjustSize(int& w, int& h, int adjustW, int adjustH);
double lerp(double v1, double v2, double sc);
double myBoundF(double min, double value, double max);
QRgb mixColor(QRgb bjRgba, QRgb qjRgba);
//inline int myGetGrid(int a, int b){
//    return a < 0 ? a / b - 1 : a / b;
//}
inline int myGetIntGrid(double a){
    return a < 0 ? (int)a - 1 : (int)a;
}

void jDrawText(QPainter *p,QPoint pos, int flags, QString str,int xLimit=-1,int yLimit=-1);
void jDrawText(QPainter *p,int x, int y, int flags, QString str,int xLimit=-1,int yLimit=-1);

void jDrawRecFrame(QPainter *p,int x,int y,int w,int h,int width,QColor c);
inline void jDrawRecFrame(QPainter *p,QRect rect,int width,QColor c){
    jDrawRecFrame(p,rect.x(),rect.y(),rect.width(),rect.height(),width,c);
}

#endif // JHEADER_H
