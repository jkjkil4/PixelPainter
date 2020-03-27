#include "mynewimagedialog.h"

MyNewImageDialog::MyNewImageDialog(int *pWidth, int *pHeight, QString *pName, QWidget *parent) : QDialog(parent) {
    this->pWidth = pWidth;
    this->pHeight = pHeight;
    this->pName = pName;
    setWindowTitle("新建");

    nameEdit->setValidator(regName);
    nameEdit->setText("新建图像");
    editWidth->setValidator(regVal);
    editWidth->setMaximumWidth(100);
    editWidth->setText("32");
    editHeight->setValidator(regVal);
    editHeight->setMaximumWidth(100);
    editHeight->setText("32");
    connect(editWidth, &QLineEdit::textChanged, [=](){
        int val = editWidth->text().toInt();
        if( val>limit )
            editWidth->setText(QString::number(limit));
    });
    connect(editHeight, &QLineEdit::textChanged, [=](){
        int val = editHeight->text().toInt();
        if( val>limit )
            editHeight->setText(QString::number(limit));
    });

    QHBoxLayout *layName = new QHBoxLayout;
    QLabel *labName = new QLabel("文件名称", this);
    labName->setMaximumWidth(50);
    labName->setMinimumWidth(50);
    layName->addWidget(labName);
    layName->addWidget(nameEdit);

    QHBoxLayout *layWidth = new QHBoxLayout;
    QLabel *labWidth = new QLabel("宽度", this);
    labWidth->setMaximumWidth(32);
    labWidth->setMinimumWidth(32);
    layWidth->setMargin(2);
    layWidth->setSpacing(2);
    layWidth->addWidget(labWidth);
    layWidth->addWidget(editWidth);

    QHBoxLayout *layHeight = new QHBoxLayout;
    QLabel *labHeight = new QLabel("高度", this);
    labHeight->setMaximumWidth(32);
    labHeight->setMinimumWidth(32);

    layHeight->setMargin(2);
    layHeight->setSpacing(2);
    layHeight->addWidget(labHeight);
    layHeight->addWidget(editHeight);

    QHBoxLayout *layBtn = new QHBoxLayout;
    layBtn->setMargin(2);
    layBtn->setSpacing(2);
    layBtn->setAlignment(Qt::AlignRight);
    layBtn->addWidget(btnAccept);
    layBtn->addWidget(btnCancel);

    QVBoxLayout *layLeft = new QVBoxLayout;
    layLeft->setMargin(2);
    layLeft->setSpacing(10);
    layLeft->addLayout(layName);
    layLeft->addWidget(new MyLine(Qt::Horizontal, this));
    layLeft->addLayout(layWidth);
    layLeft->addLayout(layHeight);
    layLeft->addWidget(new MyLine(Qt::Horizontal, this));
    layLeft->addLayout(layBtn);

    QHBoxLayout *layMain = new QHBoxLayout;
    QLabel *labInfo = new QLabel("\n宽度和高度的最大值为6000\n\n由于作者能力有限，较大的图像性能较低");
    labInfo->setMaximumWidth(60);
    labInfo->setMinimumWidth(60);
    labInfo->setAlignment(Qt::AlignTop);
    labInfo->setWordWrap(true);
    layMain->setSpacing(8);
    layMain->addLayout(layLeft);
    layMain->addWidget(new MyLine(Qt::Vertical, this));
    layMain->addWidget(labInfo);

    setLayout(layMain);

    connect(btnAccept, SIGNAL(clicked()), this, SLOT(slot_btnAccept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(slot_btnCancel()));

    adjustSize();
    setMaximumSize(width(), height());
    setMinimumSize(width(), height());
}

void MyNewImageDialog::slot_btnAccept(){
    int w = editWidth->text().toInt(), h = editHeight->text().toInt();
    QString name = nameEdit->text();
    if( w==0 || h==0 || name.trimmed()=="" )
        return;
    (*pWidth) = w;
    (*pHeight) = h;
    (*pName) = name;
    isAccept = true;
    close();
}
void MyNewImageDialog::slot_btnCancel(){
    close();
}
