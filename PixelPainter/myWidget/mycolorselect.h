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

namespace _MyColorSelect {
    class HueSelect;
    class SVSelect;
}

//色相选择
class _MyColorSelect::HueSelect : public QWidget
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *);

signals:
    void hueChanged();

public:
    explicit HueSelect(MyVars* vars, QWidget* updateWidget);
    void updateHue(int mouseY);

    MyVars* vars = nullptr;
    QWidget* updateWidget = nullptr;
};
//饱和度、明度选择
class _MyColorSelect::SVSelect : public QWidget
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *);

signals:
    void SVChanged();

public:
    explicit SVSelect(MyVars* vars);
    void updateSV(QPoint pos, bool updateAll);

    MyVars* vars = nullptr;

    QPoint posBefore;
    QPoint posNow;
    bool limitRect = false;
    static constexpr int w = 6;
};

//颜色选择
class MyColorSelect : public QGroupBox
{
    Q_OBJECT
public slots:
    void slot_onRGBChanged();
    void slot_onHSVChanged();

signals:
    void colorChanged();

public:
    explicit MyColorSelect(MyVars* vars, QWidget *parent = nullptr);
    void updateSpinBoxes();

    MyVars* vars = nullptr;

    _MyColorSelect::HueSelect* hueSelect = nullptr;
    _MyColorSelect::SVSelect* svSelect = nullptr;

    MySpinBox* spinBoxes[6];
    QLabel* labels[6];

    QTimer* limitRateHue = new QTimer(this), *limitRateSV = new QTimer(this);
    bool needUpdateHue = false, needUpdateSV = false;
};

#endif // MYCOLORSELECT_H
