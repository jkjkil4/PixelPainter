#ifndef MYUNDOANDREDO_H
#define MYUNDOANDREDO_H

#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QFileInfo>
#include <QPainter>
#include <QTime>

#include "myvars.h"

#define VERSION QDataStream::Qt_5_13

class Block
{
public:
    virtual ~Block() = 0;
    virtual bool undo() = 0;
    virtual bool redo() = 0;
    virtual void onDestroy() = 0;
};

class BlockImage : public Block
{
public:
    explicit BlockImage(MyVars *vars, QImage tmpImage, QImage layerImage, QRect rect, QString fileName, bool *flags = nullptr);
    ~BlockImage() override;
    bool undo() override;
    bool redo() override;
    void onDestroy() override;

    MyVars *vars = nullptr;

    QString fileName;

    int index = 0;
    QRect rect;
};

class MyUndoAndRedo
{
public:
    explicit MyUndoAndRedo(MyVars *vars);
    void addImage(MyVars *vars, QImage tmpImage, QImage layerImage, QRect rect, bool *flags = nullptr);
    bool undo();
    bool redo();
    void onDestroy();

    int index = 0;
    int currentIndex = -1;
    MyVars *vars = nullptr;
    QVector<Block*> blocks;
};

#endif // MYUNDOANDREDO_H
