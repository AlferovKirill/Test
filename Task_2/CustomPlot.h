#pragma once

#include <QMouseEvent>
#include <QDebug>

#include <qwt/qwt_plot.h>

#include "Ransac.h"

class CustomPlot : public QwtPlot
{
    Q_OBJECT

public:
    explicit CustomPlot(QWidget* parent = nullptr);
    Point getNewPoint();
signals:
    void readyToGiveNewPoint();
private:
    void mouseDoubleClickEvent(QMouseEvent* event) override;

    Point point;
};
