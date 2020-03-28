#ifndef MYTOOL_PEN_H
#define MYTOOL_PEN_H

#include "mytool.h"

class MyTool_Pen : public MyTool
{
public:
    explicit MyTool_Pen(MyVars *vars);
    ~MyTool_Pen() override;
    QRect _paint(int x, int y, QImage *img, QRgb col) override;
    QRegion _updateRect(QPointF mousePos, QPointF viewPos, int scale, QSize viewSize) override;
    void _drawTool(QPainter *p) override;

    MyNumScrollBar *sizeScrBar = new MyNumScrollBar;

    QRect rect;
    QRegion regBefore;
};

#endif // MYTOOL_PEN_H
