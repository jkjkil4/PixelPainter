#ifndef MYTOOL_H
#define MYTOOL_H

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "myClass/mynumscrollbar.h"

#include "myClass/myvars.h"

class MyTool
{
public:
    explicit MyTool(MyVars *vars);
    virtual ~MyTool() = 0;
    QRect paint(int x, int y, QImage *img, QRgb col, bool force = false);
    virtual QRect _paint(int x, int y, QImage *img, QRgb col) = 0;
    virtual QRegion _updateRect(QPointF mousePos, QPointF viewPos, int scale, QSize viewSize) = 0;
    virtual void _drawTool(QPainter *p) = 0;

    MyVars *vars;

    QRgb colBefore;
    QPoint posBefore;

    QLayout *layout = nullptr;
};

#endif // MYTOOL_H
