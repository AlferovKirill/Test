#include "CustomPlot.h"

CustomPlot::CustomPlot(QWidget *parent) : QwtPlot(parent) {}

Point CustomPlot::getNewPoint() {
    return point;
}

void CustomPlot::mouseDoubleClickEvent(QMouseEvent *event) {
    event->accept();

    if (event->button() == Qt::LeftButton) {
        double x = invTransform(QwtPlot::xBottom, event->x());
        double y = invTransform(QwtPlot::yLeft, event->y());

        point = Point(x, y);
        emit readyToGiveNewPoint();
    }
}
