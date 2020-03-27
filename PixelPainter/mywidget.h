#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QTimer>

class MyWidget : public QWidget
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent *);

public:
    explicit MyWidget(QWidget *parent = nullptr);

    QTimer* ptimer = new QTimer(this);

    int delay = 80;
};

#endif // MYWIDGET_H
