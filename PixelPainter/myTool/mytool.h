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
    explicit MyTool(MyVars *vars){
        this->vars = vars;
    }
    virtual ~MyTool(){}
    QRect paint(int x, int y, MyLayer *layer, QRgb col, bool force = false){
        QPoint pos = QPoint(x, y);
        if(colBefore == col && posBefore == pos && !force)
            return QRect();
        colBefore = col;
        posBefore = pos;
        return _paint(x, y, layer, col);
    }
    virtual QRect _paint(int x, int y, MyLayer *layer, QRgb col);
    virtual QRegion _updateRect(QPointF mousePos, QPointF viewPos, int scale, QSize viewSize);
    virtual void _drawTool(QPainter *p);

    MyVars *vars;

    QRgb colBefore;
    QPoint posBefore;

    QLayout *layout = nullptr;
};

#endif // MYTOOL_H
