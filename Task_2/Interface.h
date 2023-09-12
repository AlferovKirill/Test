#pragma once

#include <QMainWindow>
#include <QDebug>
#include <QTextStream>
#include <QFileDialog>
#include <QStringList>
#include <QFile>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_symbol.h>
#include <qwt/qwt_plot_magnifier.h>
#include <qwt/qwt_plot_panner.h>
#include <qwt/qwt_plot_picker.h>
#include <qwt/qwt_picker_machine.h>
#include <qwt/qwt_scale_engine.h>

#include "Ransac.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Interface; }
QT_END_NAMESPACE

class Interface : public QMainWindow {
    Q_OBJECT
public:
    Interface(QWidget *parent = nullptr);
    ~Interface();

    bool readDataFile(const QString& fileName);
    void clearAll();
public slots:
    void openDataFile();
    void fileIsNotCorrect();
    void plotUpdate();
signals:
    void fileIsNotGood();
private:
    Ui::Interface *ui;

    Coordinates* coordinates;

    QwtPlotGrid* grid;
    QwtPlotMagnifier* magnifier;
    QwtPlotPanner* panner;
    QwtPlotPicker* picker;
    QwtPlotCurve* pointsCurve;
    QwtSymbol* symbol;
};
