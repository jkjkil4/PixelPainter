#include "mytool_pen.h"

MyTool_Pen::MyTool_Pen(MyVars *vars) : MyTool(vars) {
    QHBoxLayout *lay1 = new QHBoxLayout;
    QLabel *labSize = new QLabel("大小");
    labSize->setMaximumSize(25, 15);
    labSize->setMinimumSize(25, 15);
    sizeScrBar->setMaximumHeight(15);
    sizeScrBar->setMinimumHeight(15);
    sizeScrBar->min = 1;
    sizeScrBar->max = 100;
    sizeScrBar->value = 1;
    lay1->addWidget(labSize);
    lay1->addWidget(sizeScrBar);

    QVBoxLayout *layMain = new QVBoxLayout;
    layMain->addLayout(lay1);
    layMain->addStretch();

    layout = layMain;
}
MyTool_Pen::~MyTool_Pen(){

}
QRect MyTool_Pen::_paint(int x, int y, MyLayer *layer, QRgb col){
    int width = sizeScrBar->value;
    for(int j = 0; j < width; j++){
        int pixelY = j - width/2 + y;
        for(int i = 0; i < width; i++){
            int pixelX = i - width/2 + x;
            if(pixelX >= 0 && pixelX < layer->img.width() && pixelY >= 0 && pixelY < layer->img.height()){
                QRgb drawColor = col;
                if( vars->color.isMix ){
                    QRgb bgCol = layer->img.pixel(pixelX, pixelY);
                    drawColor = mixColor(bgCol, drawColor);
                }
                layer->img.setPixel(pixelX, pixelY, drawColor);
            }
        }
    }
    return QRect(x - width/2, y - width/2, width, width);
}

//mousePos是鼠标在图像内的坐标    viewPos是图像的偏移坐标
QRegion MyTool_Pen::_updateRect(QPointF mousePos, QPointF viewPos, int scale, QSize viewSize){
    int width = sizeScrBar->value;
    int posXStart = myGetIntGrid(mousePos.x()) - width/2;
    int posYStart = myGetIntGrid(mousePos.y()) - width/2;
    int vPosX = (posXStart - viewPos.x()) * scale / 100 + viewSize.width()/2;
    int vPosY = (posYStart - viewPos.y()) * scale / 100 + viewSize.height()/2;
    int scaledWidth = width * scale / 100;
    rect = QRect(vPosX + 2, vPosY + 2, qMax(2, scaledWidth - 3), qMax(2, scaledWidth - 3));
    QRegion region = QRegion(rect) - QRegion(rect.x() + 1, rect.y() + 1, rect.width() - 2, rect.height() - 2);
    QRegion res = region + regBefore;
    regBefore = region;
    return res;
}
void MyTool_Pen::_drawTool(QPainter *p){
    jDrawRecFrame(p, rect, 1, Qt::white);
}
