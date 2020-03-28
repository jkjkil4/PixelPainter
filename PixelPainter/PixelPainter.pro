#-------------------------------------------------
#
# Project created by QtCreator 2020-03-13T08:51:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PixelPainter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        MyClass/mybutton.cpp \
        jheader.cpp \
        main.cpp \
        myClass/myline.cpp \
        myClass/mynumscrollbar.cpp \
        myClass/myspinbox.cpp \
        myClass/myundoandredo.cpp \
        myDialog/mynewimagedialog.cpp \
        myTool/mytool.cpp \
        myTool/mytool_pen.cpp \
        myWidget/myalphaselect.cpp \
        myWidget/mycolorselect.cpp \
        myWidget/myguideview.cpp \
        myWidget/mylayersview.cpp \
        myWidget/mytoolselect.cpp \
        myWidget/myviewport.cpp \
        mywidget.cpp \
        widget.cpp

HEADERS += \
        MyClass/mybutton.h \
        jheader.h \
        myClass/myline.h \
        myClass/mynumscrollbar.h \
        myClass/myspinbox.h \
        myClass/myundoandredo.h \
        myClass/myvars.h \
        myDialog/mynewimagedialog.h \
        myTool/mytool.h \
        myTool/mytool_pen.h \
        myTool/mytools.h \
        myWidget/myalphaselect.h \
        myWidget/mycolorselect.h \
        myWidget/myguideview.h \
        myWidget/mylayersview.h \
        myWidget/mytoolselect.h \
        myWidget/myviewport.h \
        mywidget.h \
        widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc
