#ifndef MYTOOLSELECT_H
#define MYTOOLSELECT_H

#include <QGroupBox>


#include "myTool/mytools.h"

class MyToolSelect : public QGroupBox
{
public:
    explicit MyToolSelect(MyVars *vars, MyTools *tools, QWidget *parent = nullptr);

    MyVars *vars = nullptr;
    MyTools *tools = nullptr;
};

#endif // MYTOOLSELECT_H
