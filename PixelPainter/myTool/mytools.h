#ifndef MYTOOLS_H
#define MYTOOLS_H

#include "mytool_pen.h"

class MyTools
{
public:
    explicit MyTools(MyVars *vars){
        tools[0] = new MyTool_Pen(vars);
    }
    MyTool* currentTool(){
        if( index>=0 && index < 1 ){
            return tools[index];
        }
        return nullptr;
    }
    void onDestroy(){
        for(int i = 0; i < 1; i++){
            delete tools[i];
        }
    }

    int index = 0;

    MyTool *tools[1];
};

#endif // MYTOOLS_H
