#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    myColorSelect = new MyColorSelect(&vars);
    myAlphaSelect = new MyAlphaSelect(&vars);
    connect(myColorSelect, &MyColorSelect::colorChanged, [=](){myAlphaSelect->otherColorChanged();});
    resize(800,608);
    menuBar->setObjectName("menuBar");
    menuBar->setStyleSheet("#menuBar{background-color:rgb(255,255,255);}");

    //布局
    QVBoxLayout* layLeft = new QVBoxLayout;
    layLeft->setSpacing(4);
    layLeft->addWidget(myColorSelect);
    layLeft->addWidget(myAlphaSelect);
    layLeft->addStretch();

    QHBoxLayout* layMain = new QHBoxLayout;
    layMain->setMargin(4);
    layMain->addLayout(layLeft);
    layMain->addStretch();

    QVBoxLayout* layAll = new QVBoxLayout;
    layAll->setMargin(0);
    layAll->setSpacing(0);
    layAll->addWidget(menuBar);
    menuBar->addAction("测试");
    layAll->addLayout(layMain);
    setLayout(layAll);
}

Widget::~Widget()
{

}
