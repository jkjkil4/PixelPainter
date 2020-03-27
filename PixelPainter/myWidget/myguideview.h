#ifndef MYGUIDEVIEW_H
#define MYGUIDEVIEW_H

#include <QGroupBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>

#include <QMouseEvent>

#include "myClass/mybutton.h"
#include "myClass/myvars.h"

namespace _MyGuideView {
    class MyImageView;
}

class _MyGuideView::MyImageView : public QWidget
{
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *);

public:
    explicit MyImageView(MyVars *vars, QWidget *parent = nullptr);

    MyVars *vars = nullptr;
};

//-------------------------------
class MyGuideView : public QGroupBox
{
public:
    explicit MyGuideView(MyVars* vars, QWidget *parent = nullptr);

    void mainImageChanged();

    MyVars* vars;

    _MyGuideView::MyImageView *view;

    MyButton* btnSmaller = new MyButton, *btnBigger = new MyButton;
    QSlider *slider = new QSlider(Qt::Orientation::Horizontal);
    QLabel *lab = new QLabel("100%");
};

#endif // MYGUIDEVIEW_H
