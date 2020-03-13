#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>

#include "myWidget/mycolorselect.h"
#include "myWidget/myalphaselect.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    MyVars vars;

    QMenuBar* menuBar = new QMenuBar;
    MyColorSelect* myColorSelect = nullptr;
    MyAlphaSelect* myAlphaSelect = nullptr;
};

#endif // MAINWINDOW_H
