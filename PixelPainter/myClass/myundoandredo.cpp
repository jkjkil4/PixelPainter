#include "myundoandredo.h"

using namespace _MyUndoAndRedo;

BlockImage::BlockImage(MyVars *vars, QImage *tmpImage, QImage *layerImage, QRect rect, QString fileName){
    this->vars = vars;
    this->fileName = fileName;
    //调整区域
    int rectLeft = rect.x();
    int rectRight = rect.x() + rect.width();
    int rectTop = rect.y();
    int rectBottom = rect.y() + rect.height();
    if(rectLeft < 0)
        rectLeft = 0;
    if(rectRight > tmpImage->width())
        rectRight = tmpImage->width();
    if(rectTop < 0)
        rectTop = 0;
    if(rectBottom > tmpImage->height())
        rectBottom = tmpImage->height();
    rect = QRect(rectLeft, rectTop, rectRight - rectLeft, rectBottom - rectTop);
    if(rect.width() <= 0 || rect.height() <= 0)
        return;
    //创建文件夹
    QDir dir;
    if(!dir.mkdir("temp"))
        return;
    //打开文件
    QFile fileRedo("temp/" + fileName + ".redo");
    if(fileRedo.open(QIODevice::WriteOnly)){
        QDataStream out(&fileRedo);
        //文件信息
        out << 0 << tmpImage->width() << tmpImage->height()
            << rect.x() << rect.y() << rect.width() << rect.height();
        //输出新的图像
        for(int j = 0; j < rect.height(); j++)
            for(int i = 0; i < rect.width(); i++)
                out << tmpImage->pixel(i + rect.x(), j + rect.y());
        //关闭文件
        fileRedo.close();
    }
    QFile fileUndo("temp/" + fileName + ".undo");
    if(fileUndo.open(QIODevice::WriteOnly)){
        QDataStream out(&fileUndo);
        //文件信息
        out << 0 << layerImage->width() << layerImage->height()
            << rect.x() << rect.y() << rect.width() << rect.height();
        //输出旧的图像
        for(int j = 0; j < rect.height(); j++)
            for(int i = 0; i < rect.width(); i++)
                out << layerImage->pixel(i + rect.x(), j + rect.y());
        //关闭文件
        fileUndo.close();
    }
}
bool BlockImage::undo(){

    return false;
}
bool BlockImage::redo(){

    return false;
}


//------------------------------
MyUndoAndRedo::MyUndoAndRedo(MyVars *vars){
    this->vars = vars;
}
void MyUndoAndRedo::onDestroy(){

}
