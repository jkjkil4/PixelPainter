#ifndef MYALPHASELECT_H
#define MYALPHASELECT_H

#include <QGroupBox>

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QMouseEvent>
#include <QPainter>

#include "../myClass/myvars.h"
#include "../myClass/myspinbox.h"
#include "jheader.h"

class MyAlphaSelect : public QGroupBox
{
private:
    //花里胡哨的透明度选择框
    class MyAlphaView : public QWidget
    {
    protected:
        void mousePressEvent(QMouseEvent *ev);
        void mouseMoveEvent(QMouseEvent *ev);
        void paintEvent(QPaintEvent *);

    public:
        MyAlphaView(MyVars* vars, MySpinBox* spinBox);
        void updateAlpha(int mouseX);

        MyVars* vars = nullptr;
        MySpinBox* spinBox = nullptr;

        int xBefore = 1, xNow = 1;
        bool limitRect = false;
    };
//--------------------------
//public slots:
//    void slot_colorChanged();

public:
    explicit MyAlphaSelect(MyVars* vars, QWidget *parent = nullptr);
    void otherColorChanged();

    QLabel* labelAlpha = new QLabel("Alpha");
    MySpinBox* spinBoxAlpha = new MySpinBox;

    MyAlphaView* view;

    MyVars* vars = nullptr;
};

#endif // MYALPHASELECT_H
