#include "myundoandredo.h"


Block::~Block(){}
BlockImage::~BlockImage(){}

BlockImage::BlockImage(MyVars *vars, QImage tmpImage, QImage layerImage, QRect rect, QString fileName, bool *flags){
    this->vars = vars;
    this->fileName = fileName;
    this->layer = vars->file.currentLayer();
    this->rect = rect;
    //调整区域
    int rectLeft = rect.x();
    int rectRight = rect.x() + rect.width();
    int rectTop = rect.y();
    int rectBottom = rect.y() + rect.height();
    if(rectLeft < 0)
        rectLeft = 0;
    if(rectRight > tmpImage.width())
        rectRight = tmpImage.width();
    if(rectTop < 0)
        rectTop = 0;
    if(rectBottom > tmpImage.height())
        rectBottom = tmpImage.height();
    rect = QRect(rectLeft, rectTop, rectRight - rectLeft, rectBottom - rectTop);
    if(rect.width() <= 0 || rect.height() <= 0)
        return;
    //创建文件夹
    if(!QFileInfo("temp").isDir()){
        QDir dir;
        if(!dir.mkdir("temp"))
            return;
    }
    //图像
    QImage imgOutput = layerImage.copy(rect);
    //打开文件
    QFile fileUndo("temp/" + fileName + ".undo");
    if(fileUndo.open(QIODevice::WriteOnly)){
        QDataStream out(&fileUndo);
        out.setVersion(VERSION);
        //输出旧的图像
        out << imgOutput;
        //关闭文件
        fileUndo.close();
    }
    if(flags){
        for(int j = 0; j < rect.height(); j++){
            for(int i = 0; i < rect.width(); i++){
                int pixelX = rect.x() + i, pixelY = rect.y() + j;
                bool has = flags[pixelX + pixelY * tmpImage.width()];
                if (has)
                    imgOutput.setPixel(i, j, tmpImage.pixel(pixelX, pixelY));
            }
        }
    }else{
        QPainter p(&imgOutput);
        p.drawImage(imgOutput.rect(), tmpImage, rect);
    }
    QFile fileRedo("temp/" + fileName + ".redo");
    if(fileRedo.open(QIODevice::WriteOnly)){
        QDataStream out(&fileRedo);
        out.setVersion(VERSION);
        //输出新的图像
        out << imgOutput;
        //关闭文件
        fileRedo.close();
    }
}
bool BlockImage::undo(){ //撤销
    QFile fileUndo("temp/" + fileName + ".undo");
    if(fileUndo.open(QIODevice::ReadOnly)){
        QImage img;
        QDataStream in(&fileUndo);
        in.setVersion(VERSION);
        //读取图像
        in >> img;
        //关闭文件
        fileUndo.close();
        //设置图像
        QImage *currentImg = &layer->img;
        for(int j = 0; j < rect.height(); j++)
            for(int i = 0; i < rect.width(); i++)
                currentImg->setPixel(rect.x() + i, rect.y() + j, img.pixel(i, j));
        return true;
    }
    return false;
}
bool BlockImage::redo(){ //重做
    QFile fileRedo("temp/" + fileName + ".redo");
    if(fileRedo.open(QIODevice::ReadOnly)){
        QImage img;
        QDataStream in(&fileRedo);
        in.setVersion(VERSION);
        //读取图像
        in >> img;
        //关闭文件
        fileRedo.close();
        //设置图像
        QImage *currentImg = &layer->img;
        for(int j = 0; j < rect.height(); j++)
            for(int i = 0; i < rect.width(); i++)
                currentImg->setPixel(rect.x() + i, rect.y() + j, img.pixel(i, j));
        return true;
    }
    return false;
}
void BlockImage::onDestroy(){
    QFile fileRedo("temp/" + fileName + ".redo");
    QFile fileUndo("temp/" + fileName + ".undo");
    fileRedo.remove();
    fileUndo.remove();
}


//------------------------------
MyUndoAndRedo::MyUndoAndRedo(MyVars *vars){
    this->vars = vars;
}
void MyUndoAndRedo::addImage(MyVars *vars, QImage tmpImage, QImage layerImage, QRect rect, bool *flags){
    if(rect.width() == 0 || rect.height() == 0)
        return;
    QTime t;
    t.start();
    int size = blocks.size();
    if(currentIndex < size - 1){
        auto itStart = blocks.begin() + (currentIndex + 1);
        for(auto it = itStart; it < blocks.end(); ++it){
            (*it)->onDestroy();
            delete *it;
        }
        blocks.erase(itStart, blocks.end());
    }
    blocks.append(new BlockImage(vars, tmpImage, layerImage, rect, QString::number(index++), flags));
    currentIndex++;
    qDebug().noquote().nospace() << "储存消耗时间:\t" << t.elapsed();
}
bool MyUndoAndRedo::undo(){
    if( currentIndex < 0 || currentIndex >= blocks.size() )
        return false;
    if(!blocks[currentIndex]->undo()){
        return false;
    }
    currentIndex--;
    qDebug()<<currentIndex;
    return true;
}
bool MyUndoAndRedo::redo(){
    currentIndex++;
    int size = blocks.size();
    if(currentIndex >= size){
        currentIndex = size - 1;
    }else{
        if(!blocks[currentIndex]->redo()){
            currentIndex--;
            return false;
        }
    }
    qDebug()<<currentIndex;
    return true;
}
void MyUndoAndRedo::onDestroy(){
    for(auto it = blocks.begin(); it != blocks.end(); ++it){
        (*it)->onDestroy();
        delete *it;
    }
}
