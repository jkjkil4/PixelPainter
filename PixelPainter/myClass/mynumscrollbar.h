#ifndef MYNUMSCROLLBAR_H
#define MYNUMSCROLLBAR_H

#include <QWidget>
#include <QTimer>

#include <QMouseEvent>

#include "jheader.h"

class MyNumScrollBar : public QWidget
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *);

public:
    explicit MyNumScrollBar(QWidget *parent = nullptr);

    void updateValue(int mouseX);

    int min = 0;
    int max = 255;
    int value = 255;

    int delay = 18;
    QTimer *limit = new QTimer(this);

signals:
    void valueChanged();
};

#endif // MYNUMSCROLLBAR_H
