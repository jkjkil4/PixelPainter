#include "widget.h"

Widget::Widget(QWidget *parent) : MyWidget(parent) {
    myColorSelect = new MyColorSelect(&vars);
    myAlphaSelect = new MyAlphaSelect(&vars);
    myToolSelect = new MyToolSelect(&vars, &tools);
    myGuideView = new MyGuideView(&vars);
    myLayersView = new MyLayersView(&vars);
    myViewport = new MyViewport(&vars, myGuideView->slider, &tools);
    connect(myColorSelect, &MyColorSelect::colorChanged, [=](){myAlphaSelect->otherColorChanged();});
    connect(myLayersView, &MyLayersView::layersChanged, [=](){myViewport->updateViewImg();});
    connect(myViewport, &MyViewport::painted, [=](){myLayersView->layersView->updateByIndex(myLayersView->layersView->currentIndex);});
    resize(800,608);
    setFocus();

    menuBar->setObjectName("menuBar");
    menuBar->setStyleSheet("#menuBar{background-color:rgb(255,255,255);}");
    QMenu* menuFile = new QMenu("文件(&F)");
    menuFile->addAction(actNewFile);
    menuFile->addAction(actOpenFile);
    menuFile->addAction(actSaveFile);
    QMenu* menuEdit = new QMenu("编辑(&E)");
    menuEdit->addAction(actUndo);
    menuEdit->addAction(actRedo);
    QMenu* menuOther = new QMenu("其他(&O)");
    menuOther->addAction(actSettings);
    menuOther->addAction(actHelp);
    menuOther->addSeparator();
    menuOther->addAction(actAbout);
    menuOther->addAction(actAboutQt);
    menuBar->addMenu(menuFile);
    menuBar->addMenu(menuEdit);
    menuBar->addMenu(menuOther);

    //布局
    QVBoxLayout* layLeft = new QVBoxLayout;
    layLeft->setSpacing(4);
    layLeft->addWidget(myColorSelect);
    layLeft->addWidget(myAlphaSelect);
    layLeft->addWidget(myToolSelect, 1);

    QVBoxLayout* layRight = new QVBoxLayout;
    layRight->setSpacing(4);
    layRight->addWidget(myGuideView);
    layRight->addWidget(myLayersView, 1);

    QHBoxLayout* layMain = new QHBoxLayout;
    layMain->setMargin(4);
    layMain->setSpacing(4);
    layMain->addLayout(layLeft);
    layMain->addWidget(myViewport, 1);
    layMain->addLayout(layRight);

    QVBoxLayout* layAll = new QVBoxLayout;
    layAll->setMargin(0);
    layAll->setSpacing(0);
    layAll->addWidget(menuBar);
    layAll->addLayout(layMain);
    setLayout(layAll);

    actNewFile->setShortcut(tr("Ctrl+N"));
    connect(actNewFile, SIGNAL(triggered()), this, SLOT(slot_newFile()));
    actOpenFile->setShortcut(tr("Ctrl+O"));
    connect(actOpenFile, SIGNAL(triggered()), this, SLOT(slot_openFile()));
    actSaveFile->setShortcut(tr("Ctrl+S"));
    connect(actSaveFile, SIGNAL(triggered()), this, SLOT(slot_saveFile()));

    actUndo->setShortcut(tr("Ctrl+Z"));
    connect(actUndo, SIGNAL(triggered()), this, SLOT(slot_undo()));
    actRedo->setShortcut(tr("Ctrl+R"));
    connect(actRedo, SIGNAL(triggered()), this, SLOT(slot_redo()));

    connect(actHelp, SIGNAL(triggered()), this, SLOT(slot_help()));
    connect(actSettings, SIGNAL(triggered()), this, SLOT(slot_settings()));
    connect(actAbout, SIGNAL(triggered()), this, SLOT(slot_about()));
    connect(actAboutQt, SIGNAL(triggered()), this, SLOT(slot_aboutQt()));
}
Widget::~Widget()
{
    vars.onDestroy();
    tools.onDestroy();
    myUndoAndRedo.onDestroy();
}

void Widget::slot_newFile(){
    int width = 0, height = 0;
    QString fileName = "";
    MyNewImageDialog dialog(&width, &height, &fileName);
    dialog.exec();
    if( dialog.isAccept ){ 
        bool ok = true;
        QString strCatch = "";
        MyLayer *layer = new MyLayer("图层1", width, height, &ok, &strCatch);
        if( !ok ){
            QMessageBox::critical(nullptr, "错误", "创建失败\n"+strCatch);
            delete layer;
            return;
        }
        vars.file.onDestroy();
        setWindowTitle(fileName);
        MyFile* file = &(vars.file);
        file->isNull = false;
        file->imageWidth = width;
        file->imageHeight = height;
        file->fileName = fileName;
        file->layers.append(layer);
        mainImageChanged();
    }
}
void Widget::slot_openFile(){
    QString path = QFileDialog::getOpenFileName(
        nullptr, "选择文件",
        "D:\\MyWonderfulExes\\jkjkil_usefulMenu\\screenShot",
        "图像(*.png *.jpg *.jpeg);;所有文件(*.*)"
    );
    if( path == "" )
        return;
    vars.file.onDestroy();
    QImage img;
    if( loadImg(path, img) ){
        QString fileName = QFileInfo(path).fileName();
        setWindowTitle(fileName);
        fileName = fileName.left(fileName.lastIndexOf("."));
        MyFile* file = &(vars.file);
        file->isNull = false;
        file->imageWidth = img.width();
        file->imageHeight = img.height();
        file->fileName = fileName;
        file->layers.append(new MyLayer(fileName, img.width(), img.height()));
        QPainter p( &( file->layers[file->layers.size()-1]->img ) );
        p.drawImage(0, 0, img);
        mainImageChanged();
    }else{
        QMessageBox::information(nullptr, "提示", "读取图像失败");
    }
}
void Widget::slot_saveFile(){
    if(vars.file.isNull)
        return;
    QString path = QFileDialog::getSaveFileName(nullptr, "保存");
    if(path == "")
        return;
    QVector<MyLayer*> layers = vars.file.layers;
    int size = vars.file.imageWidth * vars.file.imageHeight;
    QImage resultImage = QImage(vars.file.imageWidth, vars.file.imageHeight, QImage::Format_ARGB32);
    resultImage.fill(QColor(0, 0, 0, 0));
    QRgb *resultBits = reinterpret_cast<QRgb*>(resultImage.bits());
    for( auto it = layers.end() - 1; it != layers.begin() - 1; --it ){
        MyLayer *layer = *it;
        QRgb *bits = reinterpret_cast<QRgb*>(layer->img.bits());
        int layerAlpha = layer->alpha;
        for(int i = 0; i < size; i++){
            int trueLayerAlpha = layer->mask ? layerAlpha * (255 - layer->mask->mask[i]) / 255 : layerAlpha;
            QRgb qjRgba = bits[i];//前景
            QRgb bjRgba = resultBits[i];//背景
            uchar *pAlpha = reinterpret_cast<uchar*>(&qjRgba) + 3;
            *pAlpha = (uchar)(*pAlpha * trueLayerAlpha / 255);
            resultBits[i] = mixColor(bjRgba, qjRgba);
        }
    }
    resultImage.save(path);
}

void Widget::slot_undo(){

}
void Widget::slot_redo(){

}

void Widget::slot_help(){
    QMessageBox::information(nullptr, "提示", "暂无");
}
void Widget::slot_settings(){
    QMessageBox::information(nullptr, "提示", "暂无");
}
void Widget::slot_about(){
    QMessageBox::about(nullptr, "关于", "作者:\tjkjkil4"
                                "\n邮箱:\t1173374788@qq.com"
                                "\nGithub:\t暂无该项目的存储库"
                                "\n\n使用Qt制作");
}
void Widget::slot_aboutQt(){
    QMessageBox::aboutQt(nullptr);
}

bool Widget::loadImg(QString path, QImage &img){
    QImageReader reader(path);
    reader.setDecideFormatFromContent(true);
    if(reader.canRead()){
        if(reader.read(&img)){
            return true;
        }
    }
    return false;
}
void Widget::mainImageChanged(){
    QPixmap pixView = QPixmap(":/other/pic/view.png");
    MyFile *file = &vars.file;
    int imgW = file->imageWidth;
    int imgH = file->imageHeight;
    MyAdjustSize(imgW, imgH, pixView.width()-2, pixView.height()-2);
    file->adjustWidth = imgW;
    file->adjustHeight = imgH;
    //更新
    myViewport->mainImageChanged();
    myGuideView->mainImageChanged();
    myLayersView->mainImageChanged();
}

