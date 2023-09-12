#include "Interface.h"
#include "ui_Interface.h"

Interface::Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interface) {
    ui->setupUi(this);

    setWindowTitle("RANSAC Line 2D");

    coordinates = new Coordinates;

    ui->qwtPlot->setCanvasBackground(Qt::lightGray);
    ui->qwtPlot->setAutoReplot(true);

    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, "y");
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, "x");
    ui->qwtPlot->insertLegend(new QwtLegend());

    grid = new QwtPlotGrid;
    grid->setMajorPen(QPen(Qt::black, 2));
    grid->attach(ui->qwtPlot);

    magnifier = new QwtPlotMagnifier(ui->qwtPlot->canvas());
    magnifier->setMouseButton(Qt::MiddleButton);

    panner = new QwtPlotPanner(ui->qwtPlot->canvas());
    panner->setMouseButton(Qt::LeftButton);

    picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::CrossRubberBand, QwtPicker::ActiveOnly, ui->qwtPlot->canvas());

    picker->setRubberBandPen(QColor(Qt::red));
    picker->setTrackerPen(QColor(Qt::black));
    picker->setStateMachine(new QwtPickerDragPointMachine());

    connect(ui->openFile, &QAction::triggered, this, &Interface::openDataFile);
    connect(this, &Interface::fileIsNotGood, this, &Interface::fileIsNotCorrect);
}

bool Interface::readDataFile(const QString& fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QTextStream in(&file);

    bool isFileGood = true;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList coordinate = line.split(QRegExp("[\\(\\);, ]"), Qt::SkipEmptyParts);

        if (coordinate.size() != 2) {
            isFileGood = false;
            continue;
        }

        double x = coordinate[0].toDouble();
        double y = coordinate[1].toDouble();

        qDebug() << "size x string = " << coordinate[0].size() << ", size y string = " << coordinate[1].size();

        qDebug() << "x string = " << coordinate[0] << ", y string = " << coordinate[1];
        qDebug() << "x = " << x << ", y = " << y;

        if (x == 0 && coordinate[0].size() == 1 && coordinate[0][0] != '0') {
            isFileGood = false;
            continue;
        }

        if (y == 0 && coordinate[1].size() == 1 && coordinate[1][0] != '0') {
            isFileGood = false;
            continue;
        }

        coordinates->push_back(Point(x, y));
    }

    qDebug() << "coordinates size = " << coordinates->size();

    file.close();
    return isFileGood;
}

void Interface::openDataFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "", "Текстовые файлы (*.txt)");

    if (!readDataFile(fileName)) {
        emit fileIsNotGood();
    }
}

void Interface::fileIsNotCorrect() {
    qDebug() << "File is not good slot";
}

Interface::~Interface() {
    delete ui;
}

