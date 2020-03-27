#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>

#include <QMouseEvent>
#include <QTimer>

#include "../jheader.h"

class MyButton : public QWidget
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *);

public:
    explicit MyButton(QWidget *parent = nullptr);
    void limitSize(int w, int h){limitSize(QSize(w, h));}
    void limitSize(QSize size);

    int borderWidth = 2;

    int delay = 10;

    int offset1 = 10, offset2 = 15;

    bool isMouseHolding = false;
    bool isMouseAt = false;

    QColor backgroundColor = QColor(230, 230, 230);
    QColor notEnabledColor = QColor(200, 200, 200);

    QPixmap pixmap;

    QTimer *ptimer = new QTimer(this);

signals:
    void clicked();

public slots:
    void slot_ptimer();
};

#endif // MYBUTTON_H
