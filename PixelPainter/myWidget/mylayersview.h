#ifndef MYLAYERSVIEW_H
#define MYLAYERSVIEW_H

#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QScrollBar>
#include <QToolTip>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QLabel>
#include <QTimer>
#include <QMenu>

#include "myClass/mybutton.h"
#include "myClass/myvars.h"
#include "myClass/mynumscrollbar.h"
#include "jheader.h"

namespace _MyLayersView {
    class LayersView;
}

class _MyLayersView::LayersView : public QWidget
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

signals:
    void indexChanged();
    void layersChanged();

public:
    explicit LayersView(MyVars *vars, QScrollBar *scrBar, QWidget *parent = nullptr);

    void updateScrollBar();
    void selectIndex(int index);

    void updateByIndex(int index);

    MyVars *vars = nullptr;
    QScrollBar *scrBar = nullptr;

    int btnHeight = 40;
    int yOffset = 0;
    int currentIndex = -1;
    int newCount = 0;

    int tooltipIndex = -1;
    QTimer *tooltipTimer = new QTimer(this);
    QTimer *checkMouseOut = new QTimer(this);
    QString tooltipName = "";

    int spacing = 2;

    QPixmap pixView = QPixmap(":/other/pic/view.png");
};


//-------------------------------
class MyLayersView : public QGroupBox
{
    Q_OBJECT
signals:
    void layersChanged();

public:
    explicit MyLayersView(MyVars *vars, QWidget *parnet = nullptr);

    void mainImageChanged();
    void updateButtonState();

    void newLayer();
    void deleteLayer();
    void newMask();
    void deleteMask();

    MyVars *vars = nullptr;

    MyButton *btn[4]{new MyButton, new MyButton, new MyButton, new MyButton};

    MyNumScrollBar *numScrollBar = new MyNumScrollBar;

    _MyLayersView::LayersView *layersView = nullptr;
    QScrollBar *scrBar = new QScrollBar;

    QTimer *limitScrBar = new QTimer(this);
    QTimer *limitNumScrBar = new QTimer(this);
    QTimer *limitSendMessage = new QTimer(this);
};

#endif // MYLAYERSVIEW_H
