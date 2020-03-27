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

void jDrawText(QPainter *p,QPoint pos, int flags, QString str,int xLimit=-1,int yLimit=-1);
void jDrawText(QPainter *p,int x, int y, int flags, QString str,int xLimit=-1,int yLimit=-1);

void jDrawRecFrame(QPainter *p,QRect rect,int width,QColor c);
void jDrawRecFrame(QPainter *p,int x,int y,int w,int h,int width,QColor c);

#endif // JHEADER_H
