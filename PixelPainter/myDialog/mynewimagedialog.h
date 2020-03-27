#ifndef MYNEWIMAGEDIALOG_H
#define MYNEWIMAGEDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRegExpValidator>

#include "myClass/myline.h"

class MyNewImageDialog : public QDialog
{
    Q_OBJECT
public slots:
    void slot_btnAccept();
    void slot_btnCancel();

public:
    explicit MyNewImageDialog(int *width, int *height, QString *pName, QWidget *parent = nullptr);

    int *pWidth = nullptr;
    int *pHeight = nullptr;
    QString *pName = nullptr;

    int limit = 6000;

    bool isAccept = false;

    QRegExpValidator *regName = new QRegExpValidator(QRegExp("[^ \\\\/:\\*\\?\"<>|]{1}[^\\\\/:\\*\\?\"<>|]+"), this);
    QLineEdit *nameEdit = new QLineEdit(this);

    QRegExpValidator *regVal = new QRegExpValidator(QRegExp("[1-9]{1}[0-9]+"), this);
    QLineEdit *editWidth = new QLineEdit(this), *editHeight = new QLineEdit(this);

    QPushButton *btnAccept = new QPushButton("确认"), *btnCancel = new QPushButton("取消");
};

#endif // MYNEWIMAGEDIALOG_H
