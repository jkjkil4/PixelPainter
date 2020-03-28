#ifndef MYUNDOANDREDO_H
#define MYUNDOANDREDO_H

#include <QFile>
#include <QDir>
#include <QDataStream>

#include "myvars.h"

namespace _MyUndoAndRedo {
    class Block;
    class BlockImage;
}

class _MyUndoAndRedo::Block
{
public:
    virtual ~Block() = 0;
    virtual bool undo() = 0;
    virtual bool redo() = 0;
};

class _MyUndoAndRedo::BlockImage : public _MyUndoAndRedo::Block
{
public:
    explicit BlockImage(MyVars *vars, QImage *tmpImage, QImage *layerImage, QRect rect, QString fileName);
    ~BlockImage() override;
    bool undo() override;
    bool redo() override;

    MyVars *vars = nullptr;

    QString fileName;

    int index = 0;
};

class MyUndoAndRedo
{
public:
    explicit MyUndoAndRedo(MyVars *vars);
    void onDestroy();

    MyVars *vars = nullptr;
};

#endif // MYUNDOANDREDO_H
