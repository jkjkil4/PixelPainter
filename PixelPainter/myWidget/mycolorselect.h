#ifndef MYCOLORSELECT_H
#define MYCOLORSELECT_H

#include <QGroupBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QTimer>

#include <QMouseEvent>

#include "../jheader.h"
#include "../myClass/myvars.h"
#include "../myClass/myspinbox.h"

//颜色选择
class MyColorSelect : public QGroupBox
{
    Q_OBJECT
private:
    //色相选择
    class HueSelect : public QWidget
    {
    protected:
        void mousePressEvent(QMouseEvent *ev);
        void mouseMoveEvent(QMouseEvent *ev);
        void paintEvent(QPaintEvent *);

    public:
        explicit HueSelect(MyVars* vars, QWidget* updateWidget, QTimer *fakeSlot);
        void updateHue(int mouseY);

        MyVars* vars = nullptr;
        QWidget* updateWidget = nullptr;

        QTimer* fakeSlot = nullptr;
    };
    //饱和度、明度选择
    class SVSelect : public QWidget
    {
    protected:
        void mousePressEvent(QMouseEvent *ev);
        void mouseMoveEvent(QMouseEvent *ev);
        void paintEvent(QPaintEvent *);

    public:
        explicit SVSelect(MyVars* vars, QTimer *fakeSlot);
        void updateSV(QPoint pos, bool updateAll);

        MyVars* vars = nullptr;

        QPoint posBefore;
        QPoint posNow;
        bool limitRect = false;
        static constexpr int w = 6;

        QTimer* fakeSlot = nullptr;
    };

//-----------------
public slots:
    void slot_onRGBChanged();
    void slot_onHSVChanged();

signals:
    void colorChanged();

public:
    explicit MyColorSelect(MyVars* vars, QWidget *parent = nullptr);
    void updateSpinBoxes();

    MyVars* vars = nullptr;

    HueSelect* hueSelect = nullptr;
    SVSelect* svSelect = nullptr;

    MySpinBox* spinBoxes[6];
    QLabel* labels[6];

    QTimer* fakeSlotHue = new QTimer(this), *fakeSlotSV = new QTimer(this);

    QTimer* limitRateHue = new QTimer(this), *limitRateSV = new QTimer(this);
    bool needUpdateHue = false, needUpdateSV = false;
};

#endif // MYCOLORSELECT_H
