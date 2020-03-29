#include "myviewport.h"

MyViewport::MyViewport(MyVars *vars, QSlider *slider, MyTools *tools, MyUndoAndRedo *unre, QWidget *parent) : QWidget(parent) {
    this->vars = vars;
    this->slider = slider;
    this->tools = tools;
    this->unre = unre;
    setMinimumWidth(250);
    setMouseTracking(true);

    connect(slider, &QSlider::valueChanged, [=](){
        if( !limitSlider->isActive() )
            limitSlider->start(18);
    });
    limitSlider->setSingleShot(true);
    connect(limitSlider, &QTimer::timeout, [=](){
        int value = slider->value();
        if( value != scale ){
            scale = value;
            update();
        }
    });
    limitMove->setSingleShot(true);
    connect(limitMove, &QTimer::timeout, [=](){
        update();
    });
    limitPaintUpdate->setSingleShot(true);
    connect(limitPaintUpdate, &QTimer::timeout, [=](){
        update();
    });
    connect(checkMouse, &QTimer::timeout, [=](){
        if(!QRect(0, 0, width(), height()).contains(mapFromGlobal(cursor().pos()))){
            isMouseAt = false;
            checkMouse->stop();
            if(!vars->file.isNull)
                update();
        }
    });
}
MyViewport::~MyViewport(){

}

void MyViewport::wheelEvent(QWheelEvent *ev){
    if( vars->file.isNull )
        return;
    QPoint mouse = ev->pos();
    int delta = ev->angleDelta().y();
    int num = slider->value();
    int res = 100;
    if( delta<0 ){
        if( num>100 ){
            int sur = num%100;
            res = sur ? num - sur : num - 100;
            if( num>600 )
                res -= 100;
        }else if( num<10 ){
            res = slider->minimum();
        }else{
            res = num*2/3;
        }
    }else if( delta>0 ){
        if( num<100 ){
            res = num*5/3;
            if( res > 100 )
                res = 100;
        }else{
            int sur = num%100;
            res = sur ? num - sur + 100 : num + 100;
            if( num>600 )
                res += 100;
        }
    }
    scale = qBound(slider->minimum(), res, slider->maximum());
    slider->setValue( scale );
    QPointF imgPos = mousePosToImagePos(mouse);
    double sc = (double)scale/num;
    double resX = lerp(2*viewPoint.x()-imgPos.x(), viewPoint.x(), sc);
    double resY = lerp(2*viewPoint.y()-imgPos.y(), viewPoint.y(), sc);
    if( resX < 0 )
        resX = 0;
    else if( resX > vars->file.imageWidth )
        resX = vars->file.imageWidth;
    if( resY < 0 )
        resY = 0;
    else if( resY > vars->file.imageHeight )
        resY = vars->file.imageHeight;
    viewPoint = QPointF(resX, resY);
    MyTool *tool = tools->currentTool();
    if(tool)
        tool->_updateRect(mousePosToImagePos(ev->pos()), viewPoint, scale, size());
    update();
}

void MyViewport::mousePressEvent(QMouseEvent *ev){
    if( ev->button() == Qt::RightButton ){
        midButtonPoint = ev->pos();
    }else if( ev->button() == Qt::LeftButton ){
        if(!vars->file.isNull){
            MyTool* tool = tools->currentTool();
            if(tool){
                if(tempImg)
                    safeDelete(tempImg);
                if(replaceFlags)
                    safeDelete(replaceFlags);
                tempImg = new QImage(vars->file.imageWidth, vars->file.imageHeight, QImage::Format_ARGB32);
                if(tempImg->isNull()){
                    safeDelete(tempImg);
                    QMessageBox::critical(nullptr, "错误", "绘制失败");
                    return;
                }
                int size = vars->file.imageWidth * vars->file.imageHeight;
                if(vars->color.mixFlag == MyMixFlag::Replace){
                    try {
                        replaceFlags = new bool[size];
                    } catch(std::bad_alloc &mem) {
                        safeDeleteArray(replaceFlags);
                        QMessageBox::critical(nullptr, "错误", "绘制失败\n"+QString(mem.what()));
                        return;
                    }
                    for(int i = 0; i < size; i++)
                        replaceFlags[i] = false;
                }
                tempImg->fill(QColor(0, 0, 0, 0));
                QPointF posF = mousePosToImagePos(ev->pos());
                QRect rect = tool->paint(myGetIntGrid(posF.x()), myGetIntGrid(posF.y()), tempImg,
                                vars->color.getColor(), true, replaceFlags);
                setArea(rect);
                updateViewImgByRect(rect);
                if(!limitPaintUpdate->isActive())
                    limitPaintUpdate->start(18);
            }
        }
    }
}
void MyViewport::mouseMoveEvent(QMouseEvent *ev){
    if(!checkMouse->isActive()){
        isMouseAt = true;
        if(ev->buttons() == 0)
            checkMouse->start(18);
    }
    if( ev->buttons() & Qt::RightButton ){
        double deltaX = (double)(ev->x() - midButtonPoint.x())*100/scale;
        double deltaY = (double)(ev->y() - midButtonPoint.y())*100/scale;

        QPointF temp = viewPoint - QPointF(deltaX, deltaY);
        double &rx = temp.rx();
        double &ry = temp.ry();
        if( rx < 0 )
            rx = 0;
        else if( rx > vars->file.imageWidth )
            rx = vars->file.imageWidth;
        if( ry < 0 )
            ry = 0;
        else if( ry > vars->file.imageHeight )
            ry = vars->file.imageHeight;

        if( temp != viewPoint ){
            viewPoint = temp;
            if( !limitMove->isActive() )
                limitMove->start(12);
        }
        midButtonPoint = ev->pos();
    }else if( ev->buttons() & Qt::LeftButton ){
        if(!vars->file.isNull){
            QPointF posF = mousePosToImagePos(ev->pos());
            MyTool* tool = tools->currentTool();
            if(tool && tempImg){
                QRect rect = tool->paint(myGetIntGrid(posF.x()), myGetIntGrid(posF.y()), tempImg,
                                vars->color.getColor(), false, replaceFlags);
                expandArea(rect);
                updateViewImgByRect(rect);
                if(!limitPaintUpdate->isActive()){
                    limitPaintUpdate->start(18);
                }
            }
        }
    }
    MyTool *tool = tools->currentTool();
    if(tool)
        update(tool->_updateRect(mousePosToImagePos(ev->pos()), viewPoint, scale, size()));
}
void MyViewport::mouseReleaseEvent(QMouseEvent *ev){
    if(!checkMouse->isActive())
        checkMouse->start(18);
    if(ev->button() == Qt::LeftButton){
        if(!vars->file.isNull){
            if(tempImg){
                if(replaceFlags){
                    QImage *img = &vars->file.currentLayer()->img;
                    if(left != -1)
                        unre->addImage(vars, *tempImg, *img, QRect(left, top, right - left, bottom - top), replaceFlags);
                    QRgb *bits = reinterpret_cast<QRgb*>(img->bits());
                    QRgb *tmpBits = reinterpret_cast<QRgb*>(tempImg->bits());
                    int bitsSize = vars->file.imageWidth * vars->file.imageHeight;
                    for(int i = 0; i < bitsSize; i++){
                        if(replaceFlags[i]){
                            bits[i] = tmpBits[i];
                        }
                    }
                    safeDeleteArray(replaceFlags);
                    safeDelete(tempImg);
                }else{
                    QImage *img = &vars->file.currentLayer()->img;
                    if(left != -1)
                        unre->addImage(vars, *tempImg, *img, QRect(left, top, right - left, bottom - top), replaceFlags);
                    QPainter p(img);
                    p.drawImage(0, 0, *tempImg);
                    safeDelete(tempImg);
                }
            }
            emit painted();
        }
    }
}
void MyViewport::paintEvent(QPaintEvent *){
    QPainter p(this);
    //QPoint mousePos = mapFromGlobal(cursor().pos());

    if( !vars->file.isNull ){
        int imgX = width()/2 - viewPoint.x()*scale/100;
        int imgY = height()/2 - viewPoint.y()*scale/100;
        int imgW = viewImg.width()*scale/100;
        int imgH = viewImg.height()*scale/100;
        QRect imgRect = QRect(imgX, imgY, imgW, imgH);
        p.setClipRect(imgRect);
        int grayX = imgX < 0 ? -(-imgX % 16) : imgX;
        int grayY = imgY < 0 ? -(-imgY % 16) : imgY;
        int forW = (width() - grayX)/16;
        int forH = (height() - grayY)/16;
        bool isXGray = imgX < 0 ? -imgX % 32 / 16 : false;
        bool isYGray_def = imgY < 0 ? -imgY % 32 / 16 : false;
        bool isYGray = isYGray_def;
        for( int i = 0; i <= forW; i++ ){
            for( int j = 0; j <= forH; j++ ){
                int gray = isXGray==isYGray ? 204 : 255;
                p.fillRect( grayX + 16*i, grayY + 16*j, 16, 16, QColor(gray, gray, gray));
                isYGray = !isYGray;
            }
            isYGray = isYGray_def;
            isXGray = !isXGray;
        }
        p.drawImage(imgRect, viewImg, QRectF(0, 0, viewImg.width(), viewImg.height()));
        p.setClipRect(0, 0, width(), height());
        jDrawRecFrame(&p, QRect(imgRect.x()-1, imgRect.y()-1, imgRect.width()+2, imgRect.height()+2), 1, Qt::black);
        //绘制网格
        if(scale >= 800){
            QPointF startPosF = mousePosToImagePos(QPoint(0, 0));
            QPoint startPos = QPoint((int)startPosF.x(), (int)startPosF.y());
            QPointF endPosF = mousePosToImagePos(QPoint(width(), height()));
            QPoint endPos = QPoint((int)endPosF.x(), (int)endPosF.y());

            int offY = qMax(0, height()/2 - (int)(viewPoint.y()*scale/100));
            int offH = qMin(height(), height()/2 + (int)((vars->file.imageHeight - viewPoint.y()) * scale / 100))
                    - qMax(0, height()/2 - (int)(viewPoint.y() * scale / 100));
            for( int i = qMax(1, startPos.x()); i < vars->file.imageWidth && i <= endPos.x(); i++ ){
                int x = width()/2 - (viewPoint.x() - i) * scale / 100;
                p.fillRect(x, offY, 1, offH, Qt::black);
            }
            int offX = qMax(0, width()/2 - (int)(viewPoint.x()*scale/100));
            int offW = qMin(width(), width()/2 + (int)((vars->file.imageWidth - viewPoint.x()) * scale / 100))
                    - qMax(0, width()/2 - (int)(viewPoint.x() * scale / 100));
            for( int j = qMax(1, startPos.y()); j < vars->file.imageHeight && j <= endPos.y(); j++ ){
                int y = height()/2 - (viewPoint.y() - j) * scale / 100;
                p.fillRect(offX, y, offW, 1, Qt::black);
            }
        }
        //绘制工具
        if(isMouseAt){
            MyTool *tool = tools->currentTool();
            if(tool){
                p.setCompositionMode(QPainter::CompositionMode_Difference);
                tool->_drawTool(&p);
                p.setCompositionMode(QPainter::CompositionMode_SourceOver);
            }
        }
    }
    jDrawRecFrame(&p, 0, 0, width(), height(), 1, Qt::black);
}

void MyViewport::mainImageChanged(){
    MyFile *file = &vars->file;
    viewPoint = QPointF( (double)file->imageWidth/2, (double)file->imageHeight/2 );
    viewImg = QImage(file->imageWidth, file->imageHeight, QImage::Format_ARGB32);
    updateViewImg();
}
void MyViewport::updateViewImg(){
    MyFile *file = &vars->file;
    int size = file->layers.size();
    int bitsSize = file->imageWidth * file->imageHeight;
    QRgb *rgba = reinterpret_cast<QRgb*>(viewImg.bits());
    //填充
    for( int i = 0; i < bitsSize; i++ )
        rgba[i] = qRgba(0, 0, 0, 0);
    //绘制颜色
    for( int index = size - 1; index >= 0; index-- ){
        QRgb *bits = reinterpret_cast<QRgb*>(file->layers[index]->img.bits());
        MyLayer *layer = file->layers[index];
        int layerAlpha = layer->alpha;
        for( int j = 0; j < bitsSize; j++ ){
            int trueLayerAlpha = layer->mask ? layerAlpha * (255 - layer->mask->mask[j]) / 255 : layerAlpha;
            QRgb qjRgba = bits[j];//前景
            QRgb bjRgba = rgba[j];//背景
            if(index == *vars->file.currentIndex && tempImg){
                QRgb _qjRgba = mixColor(qjRgba, reinterpret_cast<QRgb*>(tempImg->bits())[j]);
                if(vars->color.mixFlag == MyMixFlag::Replace && replaceFlags){
                    if(replaceFlags[j]){
                        _qjRgba = reinterpret_cast<QRgb*>(tempImg->bits())[j];
                    }
                }
                qjRgba = _qjRgba;
            }
            uchar *pAlpha = reinterpret_cast<uchar*>(&qjRgba) + 3;
            *pAlpha = (uchar)(*pAlpha * trueLayerAlpha / 255);
            rgba[j] = mixColor(bjRgba, qjRgba);
        }
    }
    update();
}
void MyViewport::updateViewImgByRect(QRect rect){
    MyFile *file = &vars->file;
    int size = file->layers.size();
    QRgb *rgba = reinterpret_cast<QRgb*>(viewImg.bits());

    for( int j = 0; j < rect.height(); j++ ){
        int y = j + rect.y();
        if(y<0 || y>=viewImg.height())
            continue;
        for( int i = 0; i < rect.width(); i++ ){
            int x = i + rect.x();
            if(x<0 || x>=viewImg.width())
                continue;
            int pos = x + y * viewImg.width();
            rgba[pos] = qRgba(0, 0, 0, 0);
            for( int index = size - 1; index >= 0; index-- ){
                QRgb *bits = reinterpret_cast<QRgb*>(file->layers[index]->img.bits());
                MyLayer *layer = file->layers[index];
                int layerAlpha = file->layers[index]->alpha;
                int trueLayerAlpha = layer->mask ? layerAlpha * (255 - layer->mask->mask[pos]) / 255 : layerAlpha;
                QRgb qjRgba = bits[pos];//前景
                QRgb bjRgba = rgba[pos];//背景
                if(index == *vars->file.currentIndex && tempImg){
                    QRgb _qjRgba = mixColor(qjRgba, reinterpret_cast<QRgb*>(tempImg->bits())[pos]);
                    if(vars->color.mixFlag == MyMixFlag::Replace && replaceFlags){
                        if(replaceFlags[pos]){
                            _qjRgba = reinterpret_cast<QRgb*>(tempImg->bits())[pos];
                        }
                    }
                    qjRgba = _qjRgba;
                }
                uchar *pAlpha = reinterpret_cast<uchar*>(&qjRgba) + 3;
                *pAlpha = (uchar)(*pAlpha * trueLayerAlpha / 255);
                rgba[pos] = mixColor(bjRgba, qjRgba);
            }
        }
    }
}

QPointF MyViewport::mousePosToImagePos(QPoint mouse){
    if( vars->file.isNull )
        return QPointF(0,0);
    double posXToCenter = mouse.x() - (double)width()/2;
    double posYToCenter = mouse.y() - (double)height()/2;
    double posXToImg = posXToCenter*100/scale + viewPoint.x();
    double posYToImg = posYToCenter*100/scale + viewPoint.y();
    return QPointF(posXToImg, posYToImg);
}

void MyViewport::setArea(QRect rect){
    //图像大小
    int imageWidth = vars->file.imageWidth;
    int imageHeight = vars->file.imageHeight;
    //区域
    int tmpLeft = qBound(0, rect.x(), imageWidth);
    int tmpRight = qBound(0, rect.x() + rect.width(), imageWidth);
    int tmpTop = qBound(0, rect.y(), imageHeight);
    int tmpBottom = qBound(0, rect.y() + rect.height(), imageHeight);
    //判断
    if(tmpLeft != tmpRight && tmpTop != tmpBottom){
        left = tmpLeft;
        right = tmpRight;
        top = tmpTop;
        bottom = tmpBottom;
    }else{
        left = -1;
    }
}
void MyViewport::expandArea(QRect rect){
    if(left == -1){
        setArea(rect);
        return;
    }
    //图像大小
    int imageWidth = vars->file.imageWidth;
    int imageHeight = vars->file.imageHeight;
    //区域
    int tmpLeft = qBound(0, rect.x(), imageWidth);
    int tmpRight = qBound(0, rect.x() + rect.width(), imageWidth);
    int tmpTop = qBound(0, rect.y(), imageHeight);
    int tmpBottom = qBound(0, rect.y() + rect.height(), imageHeight);
    //判断
    if(tmpLeft != tmpRight && tmpTop != tmpBottom){
        if(tmpRight > 0)
            left = qMin(tmpLeft, left);
        if(tmpLeft < imageWidth)
            right = qMax(right, tmpRight);
        if(tmpBottom > 0)
            top = qMin(tmpTop, top);
        if(tmpTop < imageHeight)
            bottom = qMax(bottom, tmpBottom);
    }
}
