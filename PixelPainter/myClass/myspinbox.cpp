#include "myspinbox.h"

MySpinBox::MySpinBox(QWidget *parent) : QSpinBox(parent){
    mySetMax(maximum());
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(slot_valueChanged(int)));
}

void MySpinBox::mySetValue(int value){
    blockSignals(true);
    setValue(qBound(minimum(), value, maximum()));
    blockSignals(false);
}
void MySpinBox::mySetMax(int max){
    realMax = max;
    QString resMax=QString('9').repeated(QString::number(max).length()+1);
    setMaximum(resMax.toInt());
}

void MySpinBox::slot_valueChanged(int value){
    if( value>realMax ){
        blockSignals(true);
        setValue(realMax);
        blockSignals(false);
    }
}
