#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QImageReader>
#include <QFileInfo>
#include <QProcess>

#include "myWidget/mycolorselect.h"
#include "myWidget/myalphaselect.h"
#include "myWidget/mytoolselect.h"

#include "myWidget/myguideview.h"
#include "myWidget/mylayersview.h"

#include "myWidget/myviewport.h"

#include "mywidget.h"
#include "myDialog/mynewimagedialog.h"
#include "myClass/myundoandredo.h"

class Widget : public MyWidget
{
    Q_OBJECT

public slots:
    void slot_newFile();
    void slot_openFile();
    void slot_saveFile();

    void slot_undo();
    void slot_redo();

    void slot_help();
    void slot_settings();
    void slot_about();
    void slot_aboutQt();

public:
//    void keyPressEvent(QKeyEvent *){
//        //MyColor* color = &(vars.color);
//        qd<< sizeof(vars);
//    }
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    bool loadImg(QString path, QImage& img);
    void mainImageChanged();

    long long pid;
    MyVars vars;
    MyTools tools = MyTools(&vars);
    MyUndoAndRedo myUndoAndRedo = MyUndoAndRedo(&vars);


    QMenuBar* menuBar = new QMenuBar;

    MyColorSelect* myColorSelect = nullptr;
    MyAlphaSelect* myAlphaSelect = nullptr;
    MyToolSelect* myToolSelect = nullptr;

    MyGuideView* myGuideView = nullptr;
    MyLayersView* myLayersView = nullptr;

    MyViewport* myViewport = nullptr;

    //菜单栏(文件)
    QAction *actNewFile = new QAction("新建文件(&N)");
    QAction *actOpenFile = new QAction("打开文件(&O)");
    QAction *actSaveFile = new QAction("保存文件(&S)");
    //菜单栏(编辑)
    QAction *actUndo = new QAction("撤销(&U)");
    QAction *actRedo = new QAction("重做(&R)");
    //菜单栏(其他)
    QAction *actSettings = new QAction("选项(&O)");
    QAction *actHelp = new QAction("帮助(&H)");
    QAction *actAbout = new QAction("关于(&A)");
    QAction *actAboutQt = new QAction("关于Qt(&Q)");

    int moveStep = 15;
};

#endif // MAINWINDOW_H
