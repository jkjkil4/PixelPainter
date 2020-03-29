#include "mytool.h"

MyTool::MyTool(MyVars *vars){
    this->vars = vars;
}
MyTool::~MyTool(){}

QRect MyTool::paint(int x, int y, QImage *img, QRgb col, bool force, bool *flags){
    QPoint pos = QPoint(x, y);
    if(colBefore == col && posBefore == pos && !force)
        return QRect();
    colBefore = col;
    posBefore = pos;
    return _paint(x, y, img, col, flags);
}
