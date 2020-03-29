#ifndef MYVIEWPORT_H
#define MYVIEWPORT_H

#include <QWidget>
#include <QSlider>
#include <QTimer>
#include <QMouseEvent>
#include <QTime>
#include <QMessageBox>
#include <cmath>

#include <QMouseEvent>

#include "myClass/myvars.h"
#include "jheader.h"

#include "myTool/mytools.h"
#include "myClass/myundoandredo.h"

class MyViewport : public QWidget
{
    Q_OBJECT
protected:
    void wheelEvent(QWheelEvent *ev);
    //void resizeEvent(QResizeEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *);

signals:
    void painted();

public:
    explicit MyViewport(MyVars *vars, QSlider *slider, MyTools *tools, MyUndoAndRedo *unre, QWidget *parent = nullptr);
    ~MyViewport();

    void mainImageChanged();
    void updateViewImg();
    void updateViewImgByRect(QRect rect);

    QPointF mousePosToImagePos(QPoint mouse);

    void setArea(QRect rect);
    void expandArea(QRect rect);

    MyVars *vars = nullptr;
    QSlider *slider = nullptr;
    MyUndoAndRedo *unre = nullptr;

    QTimer *limitSlider = new QTimer(this);
    QTimer *limitMove = new QTimer(this);
    QTimer *limitPaintUpdate = new QTimer(this);
    QTimer *checkMouse = new QTimer(this);

    int scale = 100;

    QPointF viewPoint;

    QPoint midButtonPoint;

    int blockWidth = 16;

    QImage viewImg;
    QImage *tempImg = nullptr;
    bool *replaceFlags;
    int top = 0, left = 0, right = 0, bottom = 0;

    MyTools *tools = nullptr;

    bool isMouseAt = false;

};

#endif // MYVIEWPORT_H
