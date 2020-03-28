#include "mylayersview.h"

using namespace _MyLayersView;



LayersView::LayersView(MyVars *vars, QScrollBar *scrBar, QWidget *parent) : QWidget(parent){
    this->vars = vars;
    this->scrBar = scrBar;
    vars->file.currentIndex = &currentIndex;
    setMouseTracking(true);
    updateScrollBar();

    tooltipTimer->setSingleShot(true);
    connect(tooltipTimer, &QTimer::timeout, [=](){
        if( QRect(0, 0, width(), height()).contains(mapFromGlobal(cursor().pos())) ){
            QToolTip::showText(cursor().pos(), tooltipName);
        }
    });
    connect(checkMouseOut, &QTimer::timeout, [=](){
        if( !QRect(0, 0, width(), height()).contains(mapFromGlobal(cursor().pos())) ){
            checkMouseOut->stop();
            tooltipIndex = -1;
        }
    });
}
void LayersView::updateScrollBar(){
    int size = vars->file.layers.size();
    int hei = btnHeight * size;
    if( hei > height() ){
        scrBar->setMaximum(hei - height());
        scrBar->setEnabled(true);
        if( yOffset > scrBar->maximum() )
            yOffset = scrBar->maximum();
    }else{
        scrBar->setEnabled(false);
        yOffset = 0;
    }
    update();
}
void LayersView::selectIndex(int index){
    int size = vars->file.layers.size();
    int hei = size * btnHeight;
    if( hei > height() ){
        int min = (index+1) * btnHeight - height();
        int max = index * btnHeight;
        scrBar->setValue( qBound(min, scrBar->value(), max) );
    }
}
void LayersView::updateByIndex(int index){
    int off = btnHeight * index - yOffset;
    if( off < height() && off + btnHeight > 0 ){
        update(0, off, width(), btnHeight);
    }
}
void LayersView::mousePressEvent(QMouseEvent *ev){
    if( ev->button()==Qt::MidButton )
        return;
    int size = vars->file.layers.size();
    int index = (ev->y() + yOffset) / btnHeight;
    if( size==0 )
        return;
    if( index < 0 || index >= size )
        return;
    currentIndex = index;
    selectIndex(currentIndex);
    emit indexChanged();
    if( ev->button()==Qt::RightButton ){
        if( index < 0 || index >= size )
            return;
        tooltipTimer->stop();
        QMenu menu;
        QAction actCancel("取消");
        QAction actMoveUp("上移一层");
        QAction actMoveDown("下移一层");
        QAction actMoveAbove("移至顶层");
        QAction actMoveBelow("移至底层");

        if( index <= 0 ){
            actMoveUp.setEnabled(false);
            actMoveAbove.setEnabled(false);
        }
        if( index >= size - 1 ){
            actMoveDown.setEnabled(false);
            actMoveBelow.setEnabled(false);
        }

        menu.addAction(&actCancel);
        menu.addSeparator();
        menu.addAction(&actMoveUp);
        menu.addAction(&actMoveDown);
        menu.addSeparator();
        menu.addAction(&actMoveAbove);
        menu.addAction(&actMoveBelow);
        menu.addSeparator();

        QAction *res = menu.exec(cursor().pos());

        //上移
        if( res == &actMoveUp ){
            MyFile *file = &vars->file;
            MyLayer *temp = file->layers[index];
            file->layers[index] = file->layers[index - 1];
            file->layers[index - 1] = temp;
            selectIndex(index - 1);
            currentIndex = index - 1;
            update();
            emit layersChanged();
        //下移
        }else if( res == &actMoveDown ){
            MyFile *file = &vars->file;
            MyLayer *temp = file->layers[index];
            file->layers[index] = file->layers[index + 1];
            file->layers[index + 1] = temp;
            selectIndex(index + 1);
            currentIndex = index + 1;
            update();
            emit layersChanged();
        //移至顶层
        }else if( res == &actMoveAbove ){
            MyFile *file = &vars->file;
            MyLayer *temp = file->layers[index];
            //0~index区间内的全部下移一格
            for( int i=index; i>0; i-- ){
                file->layers[i] = file->layers[i - 1];
            }
            file->layers[0] = temp;
            selectIndex(0);
            currentIndex = 0;
            update();
            emit layersChanged();
        //移至底层
        }else if( res == &actMoveBelow ){
            MyFile *file = &vars->file;
            MyLayer *temp = file->layers[index];
            //index~size-1区间内的全部上移一格
            for( int i=index; i<size - 1; i++ ){
                file->layers[i] = file->layers[i + 1];
            }
            file->layers[size - 1] = temp;
            selectIndex(size - 1);
            currentIndex = size - 1;
            update();
            emit layersChanged();
        }

    }
}
void LayersView::mouseMoveEvent(QMouseEvent *ev){
    if( !checkMouseOut->isActive() ){
        checkMouseOut->start(18);
    }
    int size = vars->file.layers.size();
    if( size==0 )
        return;
    int sub = (ev->y() + yOffset) % btnHeight;
    if( sub > 1 && sub < btnHeight ){
        int index = (ev->y() + yOffset) / btnHeight;
        if( index < 0 || index >= size ){
            tooltipIndex = -1;
            tooltipTimer->stop();
            return;
        }
        //if( tooltipIndex!=-1 ){
            QString name = vars->file.layers[index]->name;
            tooltipName = name;
            if( tooltipIndex != index ){
                tooltipTimer->start(500);
            }
        //}
        tooltipIndex = index;
    }
}
void LayersView::wheelEvent(QWheelEvent *ev){
    int max = vars->file.layers.size()*btnHeight;
    if( max < height() )
        return;
    int off = -(ev->angleDelta().y()/8);
    yOffset += off;
    yOffset = qBound(0, yOffset, max - height());
    scrBar->setValue(yOffset);
    update();
}
void LayersView::paintEvent(QPaintEvent *){
    QPainter p(this);

    int size = vars->file.layers.size();
    int startY = yOffset/btnHeight;
    int endY = (height()+yOffset)/btnHeight;
    for( int i=startY; i<=endY; i++ ){
        if( i>=size )
            break;
        MyLayer* layer = vars->file.layers[i];
        int offset = i*btnHeight - yOffset;
        int adW = vars->file.adjustWidth, adH = vars->file.adjustHeight;
        //绘制背景
        p.setClipRect(0, 0, width(), height());
        p.fillRect(2, 2 + offset, width()-4, btnHeight-4,
                   currentIndex==i ? QColor(170,170,255) : Qt::lightGray);
        //绘制图像(图层)
        QRect rect = QRect(21 + (pixView.width()-adW)/2, 4 + (pixView.height()-adH)/2 + offset, adW, adH);
        p.setClipRect(21, 4+offset, pixView.width(), pixView.height());
        p.drawPixmap(21, 4 + offset, pixView);
        //p.drawPixmap(21 + (pixView.width()-adW)/2,
        //             4 + (pixView.height()-adH)/2 + offset, layer->smallPix);
        p.drawImage(rect, layer->img, layer->img.rect());
        //绘制图像(蒙版)
        if( layer->mask ){
            QRect rect = QRect(width()-36, 4 + offset, pixView.width(), pixView.height());
            p.setClipRect(rect);
            //p.drawPixmap(width()-36, 4 + offset, pixView);
            jDrawRecFrame(&p, rect, 1, Qt::black);
            p.fillRect(rect.x()+1, rect.y()+1, rect.width()-2, rect.height()-2, Qt::gray);
//            p.fillRect(rect.x()+(rect.width() - adW)/2, rect.y()+(rect.height() - adH)/2, adW, adH, Qt::black);
            p.drawPixmap( rect.x()+(rect.width() - adW)/2, rect.y()+(rect.height() - adH)/2, layer->smallMask);
        }
        //绘制文字
        int rLimit = (layer->mask ? 79 : 47);
        p.setClipRect(56, 6+offset, width() - rLimit - 15, btnHeight);
        jDrawText(&p, 56, 6+offset, Qt::AlignLeft | Qt::AlignTop, layer->name);
        jDrawText(&p, 56, offset + btnHeight - 5,
                  Qt::AlignLeft | Qt::AlignBottom, QString::number(layer->alpha*100/255)+"%");
    }
    p.setClipRect(0, 0, width(), height());

    jDrawRecFrame(&p, 0, 0, width(), height(), 1, Qt::darkGray);
}
void LayersView::resizeEvent(QResizeEvent *){
    updateScrollBar();
}

//-----------------------------
MyLayersView::MyLayersView(MyVars *vars, QWidget *parent) : QGroupBox(parent){
    this->vars = vars;
    setTitle("图层");
    setMaximumWidth(210);
    setMinimumWidth(210);

    btn[0]->pixmap = QPixmap(":/layers/pic/addLayer.png");
    btn[0]->setToolTip("新建图层");
    btn[0]->setEnabled(false);
    connect(btn[0], &MyButton::clicked, [=](){newLayer();});
    btn[1]->pixmap = QPixmap(":/layers/pic/deleteLayer.png");
    btn[1]->setToolTip("删除图层");
    btn[1]->setEnabled(false);
    connect(btn[1], &MyButton::clicked, [=](){deleteLayer();});
    btn[2]->pixmap = QPixmap(":/layers/pic/addMask.png");
    btn[2]->setToolTip("创建图层蒙版");
    btn[2]->setEnabled(false);
    connect(btn[2], &MyButton::clicked, [=](){newMask();});
    btn[3]->pixmap = QPixmap(":/layers/pic/deleteMask.png");
    btn[3]->setToolTip("删除图层蒙版");
    btn[3]->setEnabled(false);
    connect(btn[3], &MyButton::clicked, [=](){deleteMask();});
    for( int i=0; i<4; i++ ){
        btn[i]->limitSize(28, 28);
    }

    numScrollBar->setMaximumHeight(15);
    numScrollBar->setMinimumHeight(15);
    numScrollBar->setEnabled(false);

    layersView = new LayersView(vars, scrBar);

    //布局
    QHBoxLayout *lay1 = new QHBoxLayout;
    lay1->setMargin(2);
    lay1->setSpacing(2);
    lay1->addWidget(btn[0]);
    lay1->addWidget(btn[1]);
    lay1->addStretch();
    lay1->addWidget(btn[2]);
    lay1->addWidget(btn[3]);

    QHBoxLayout *lay2 = new QHBoxLayout;
    QLabel *labAlpha = new QLabel("透明度");
    labAlpha->setMaximumSize(40, 15);
    labAlpha->setMinimumSize(40, 15);
    lay2->addWidget(labAlpha);
    lay2->addWidget(numScrollBar);

    QHBoxLayout *lay3 = new QHBoxLayout;
    lay3->setMargin(2);
    lay3->setSpacing(0);
    lay3->addWidget(layersView, 1);
    lay3->addWidget(scrBar);

    QVBoxLayout *layMain = new QVBoxLayout;
    layMain->setMargin(4);
    layMain->setSpacing(3);
    layMain->addLayout(lay1);
    layMain->addLayout(lay2);
    layMain->addLayout(lay3, 1);

    setLayout(layMain);

    limitScrBar->setSingleShot(true);
    connect(limitScrBar, &QTimer::timeout, [=](){
        layersView->yOffset = scrBar->value();
        layersView->update();
    });
    limitNumScrBar->setSingleShot(true);
    connect(limitNumScrBar, &QTimer::timeout, [=](){
        vars->file.layers[ layersView->currentIndex ]->alpha = numScrollBar->value;
        int index = layersView->currentIndex;
        if( index >= 0 && index < vars->file.layers.size() )
            layersView->updateByIndex( index );
    });
    limitSendMessage->setSingleShot(true);
    connect(limitSendMessage, &QTimer::timeout, [=](){
        emit layersChanged();
    });

    connect(scrBar, &QScrollBar::valueChanged, [=](){
        if( !limitScrBar->isActive() )
            limitScrBar->start(12);
    });
    connect(numScrollBar, &MyNumScrollBar::valueChanged, [=](){
        if( !limitNumScrBar->isActive() )
            limitNumScrBar->start(18);
        if( !limitSendMessage->isActive() )
            limitSendMessage->start(200);
    });
    connect(layersView, &LayersView::indexChanged, [=](){
        updateButtonState();
    });

    connect(layersView, &LayersView::layersChanged, [=](){emit layersChanged();});
}
void MyLayersView::mainImageChanged(){
    layersView->currentIndex = 0;
    layersView->yOffset = 0;
    QPixmap pixView = QPixmap(":/other/pic/view.png");
    int w = pixView.width(), h = pixView.height();
    int adW = vars->file.adjustWidth, adH = vars->file.adjustHeight;
    int subW = (w - adW)/2 -1,
        subH = (h - adH)/2 -1;
    QPainter p(&pixView);
    if( subH!=h-2 ){
        p.fillRect(1, 1, w-2, subH, Qt::gray);
        p.fillRect(1, h-1, w-2, -subH-1, Qt::gray);
    }
    if( subW!=w-2 ){
        p.fillRect(1, 1, subW, h-2, Qt::gray);
        p.fillRect(w-1, 1, -subW-1, h-2, Qt::gray);
    }
    vars->file.layers[0]->updateSmallPix(adW, adH);
    layersView->pixView = pixView;
    updateButtonState();
}
void MyLayersView::updateButtonState(){
    int index = layersView->currentIndex;
    int size = vars->file.layers.size();
    btn[0]->setEnabled( index!=-1 );
    btn[1]->setEnabled( index!=-1 && size>1 );
    if( size!=0 && index>=0 && index<size ){
        bool hasMask = vars->file.layers[index]->mask;
        btn[2]->setEnabled( !hasMask );
        btn[3]->setEnabled( hasMask );
        numScrollBar->setEnabled(true);
        numScrollBar->value = vars->file.layers[index]->alpha;
        numScrollBar->update();
    }else{
        numScrollBar->setEnabled(false);
        numScrollBar->value = 255;
        numScrollBar->update();
    }
    layersView->updateScrollBar();
}
void MyLayersView::newLayer(){
    MyFile* file = &vars->file;
    int max = 0;
    int size = file->layers.size();
    for( int i=0; i<size; i++ ){
        QString str = file->layers[i]->name;
        if( str.length()>2 ){
            int id = str.right(str.length() - 2).toInt();
            if( id>max )
                max = id;
        }
    }
    bool ok = true;
    QString strCatch;
    MyLayer *layer = new MyLayer("图层"+QString::number(max + 1), file->imageWidth, file->imageHeight, &ok, &strCatch);
    if( !ok ){
        QMessageBox::critical(nullptr, "错误", "图层创建失败\n"+strCatch);
        safeDelete(layer);
        return;
    }
    vars->file.layers.insert(layersView->currentIndex, layer);
    layersView->selectIndex(layersView->currentIndex);
    updateButtonState();
    emit layersChanged();
}
void MyLayersView::deleteLayer(){
    int size = vars->file.layers.size();
    if( size <=1 )
        return;
    int index = layersView->currentIndex;
    if( index >= 0 && index < size ){
        vars->file.layers[index]->onDestroy();
        vars->file.layers.remove(index);
    }
    if( index >= size - 1 ){
        layersView->currentIndex--;
    }
    layersView->selectIndex(layersView->currentIndex);
    updateButtonState();
    emit layersChanged();
}
void MyLayersView::newMask(){
    int size = vars->file.layers.size();
    int index = layersView->currentIndex;
    if( index >= 0 && index < size ){
        MyLayer* MyLayer = vars->file.layers[index];
        if( !MyLayer->mask ){
            MyLayer->initMask();
            MyLayer->updateSmallPix(vars->file.adjustWidth, vars->file.adjustHeight);
        }
    }
    updateButtonState();
    emit layersChanged();
}
void MyLayersView::deleteMask(){
    int index = layersView->currentIndex;
    int size = vars->file.layers.size();
    if( index >= 0 && index < size ){
        MyLayer *layer = vars->file.layers[index];
        if( layer->mask ){
            layer->mask->onDestroy();
            layer->mask = nullptr;
            updateButtonState();
        }
    }
    emit layersChanged();
}


