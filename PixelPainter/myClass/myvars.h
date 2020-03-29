#ifndef MYVARS_H
#define MYVARS_H

#include <QImage>
#include <QPixmap>

#include <QDebug>

enum MyMixFlag{ Mix, MixAndThicker, Replace };

class MyColor
{
public:
    int solidR = 255;
    int solidG = 0;
    int solidB = 0;

    int h = 0;
    int s = 255;
    int v = 255;

    int r = 255;
    int g = 0;
    int b = 0;

    int alpha = 255;

    int mixFlag = MyMixFlag::Mix;

    QRgb getColor(){
        return qRgba(r, g, b, alpha);
    }
};

class MyMask
{
public:
    explicit MyMask(int w, int h){
        this->w = w;
        this->h = h;
        int size = w*h;
        mask = new uchar[size];
        for( int i=0; i<size; i++ ){
            mask[i] = 255;
        }
        qDebug().noquote().nospace()<<"创建了一个蒙版!\t大小: "<<w<<" "<<h;
    }
    void onDestroy(){
        delete[] mask;
        qDebug().noquote().nospace()<<"销毁了一个蒙版!\t大小: "<<w<<" "<<h;
        delete this;
    }

    uchar* mask = nullptr;

    int w = -1;
    int h = -1;
};
class MyLayer
{
public:
    explicit MyLayer(QString name, int w, int h, bool *ok = nullptr, QString *strCatch = nullptr, QColor imgColor = QColor(0,0,0,0)){
        this->name = name;
        this->w = w;
        this->h = h;
        try {
            pixels = new QRgb[w*h];
        }catch(std::bad_alloc &memExp){
            (*strCatch) = memExp.what();
            if(ok)
                *ok = pixels;
            return;
        }
        img = QImage(reinterpret_cast<uchar*>(pixels), w, h, QImage::Format_ARGB32);
        img.fill(imgColor);
        qDebug().noquote().nospace()<<"创建了一个图层!\t大小: "<<w<<" "<<h;
    }
    void initMask(){
        mask = new MyMask(w, h);
    }
    void onDestroy(){
        if(mask){
            mask->onDestroy();
            mask = nullptr;
        }
        delete[] pixels;
        qDebug().noquote().nospace()<<"销毁了一个图层!\t大小: "<<w<<" "<<h;
        delete this;
    }
    void updateSmallPix(int adW, int adH){
        //smallPix = QPixmap::fromImage(img).scaled(adW, adH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        if(mask){
            int size = w*h;
            uchar *bits = mask->mask;
            uchar *argb = new uchar[size*4];
            for( int i=0; i<size; i++ ){
                uchar ch = 255 - bits[i];
                int startIndex = 4*i;
                for( int j=0; j<3; j++ ){
                    argb[startIndex + j] = ch;
                }
                argb[startIndex + 3] = 255;
            }
            QImage tempImg(argb, w, h, QImage::Format_ARGB32);
            smallMask = QPixmap::fromImage(tempImg).scaled(adW, adH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            delete[] argb;
        }
    }
    int alpha = 255;
    
    MyMask* mask = nullptr;
    QRgb* pixels = nullptr;
    QImage img;

    //QPixmap smallPix;
    QPixmap smallMask;

    QString name = "";

    int w = -1;
    int h = -1;
};

class MyFile
{
public:
    void onDestroy(){
        for( int i=0; i<layers.size(); i++ ){
            layers[i]->onDestroy();
            layers[i] = nullptr;
        }
        layers.clear();
    }

    bool isNull = true;

    int imageWidth = -1;
    int imageHeight = -1;

    QString fileName = "";

    QRectF viewRect;

    QVector<MyLayer*> layers;

    int adjustWidth = -1;
    int adjustHeight = -1;

    int *currentIndex = nullptr;
    MyLayer* currentLayer(){
        return isNull ? nullptr : layers[*currentIndex];
    }
};

class MyVars
{
public:
    void onDestroy(){
        file.onDestroy();
    }

    MyColor color;

    MyFile file;
};

#endif // MYVARS_H
