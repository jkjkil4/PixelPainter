#ifndef MYLINE_H
#define MYLINE_H

#include <QWidget>
#include <QPainter>

class MyLine : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent *);

public:
    explicit MyLine(int type, QWidget *parent = nullptr);

    void updateState();
    void setWidth(int w);
    void setColor(QColor c);

    int w = 1;

    QColor color = Qt::darkGray;

    int type = 0;
};

#endif // MYLINE_H
