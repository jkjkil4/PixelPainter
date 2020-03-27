#include "mytoolselect.h"

MyToolSelect::MyToolSelect(MyVars *vars, MyTools *tools, QWidget *parent) : QGroupBox(parent) {
    this->vars = vars;
    this->tools = tools;
    setTitle("工具");
    setMaximumWidth(183);
    setMinimumSize(183, 40);

    setLayout(tools->currentTool()->layout);
}
