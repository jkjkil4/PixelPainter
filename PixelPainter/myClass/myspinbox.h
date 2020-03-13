#ifndef MYSPINBOX_H
#define MYSPINBOX_H

#include <QSpinBox>

class MySpinBox : public QSpinBox
{
    Q_OBJECT
public:
    MySpinBox(QWidget *parent = nullptr);
    void mySetValue(int value);
    void mySetMax(int max);

    int realMax = 99;

public slots:
    void slot_valueChanged(int value);
};

#endif // MYSPINBOX_H
