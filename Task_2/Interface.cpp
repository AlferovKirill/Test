#include "Interface.h"
#include "ui_Interface.h"

Interface::Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interface) {
    ui->setupUi(this);

    setWindowTitle("RANSAC Line 2D");

    coordinates = new Coordinates;
    qwtPlot = new CustomPlot;

    vBoxLayout = new QVBoxLayout(ui->customPlot);
    vBoxLayout->addWidget(qwtPlot);

    qwtPlot->setCanvasBackground(Qt::white);
    qwtPlot->setAutoReplot(true);
    qwtPlot->setAxisTitle(QwtPlot::yLeft, "y");
    qwtPlot->setAxisTitle(QwtPlot::xBottom, "x");
    qwtPlot->insertLegend(new QwtLegend());

    grid = new QwtPlotGrid;
    grid->setMajorPen(QPen(Qt::black, 2));
    grid->attach(qwtPlot);

    magnifier = new QwtPlotMagnifier(qwtPlot->canvas());
    magnifier->setMouseButton(Qt::MiddleButton);

    panner = new QwtPlotPanner(qwtPlot->canvas());
    panner->setMouseButton(Qt::LeftButton);

    picker = new QwtPlotPicker(QwtPlot::xTop, QwtPlot::yLeft, QwtPlotPicker::CrossRubberBand, QwtPicker::ActiveOnly, qwtPlot->canvas());
    picker->setRubberBandPen(QColor(Qt::red));
    picker->setTrackerPen(QColor(Qt::black));
    picker->setStateMachine(new QwtPickerDragPointMachine());

    pointsCurve = new QwtPlotCurve;
    answerPointsCurve = new QwtPlotCurve;
    answerCurve = new QwtPlotCurve;

    pointsSymbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::gray), QPen(Qt::gray, 2), QSize(5, 5));
    answerPointsSymbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::blue), QPen(Qt::blue, 2), QSize(5, 5));
    answerSymbol = new QwtSymbol;

    pointsCurve->setSymbol(pointsSymbol);
    answerPointsCurve->setSymbol(answerPointsSymbol);
    answerCurve->setSymbol(answerSymbol);

    pointsCurve->setStyle(QwtPlotCurve::NoCurve);
    pointsCurve->setLegendAttribute(QwtPlotCurve::LegendShowBrush, true);

    answerPointsCurve->setStyle(QwtPlotCurve::NoCurve);
    answerPointsCurve->setLegendAttribute(QwtPlotCurve::LegendShowBrush, true);

    answerCurve->setPen(Qt::blue, 3);
    answerCurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    answerCurve->setLegendAttribute(QwtPlotCurve::LegendShowBrush, true);

    pointsCurve->attach(qwtPlot);
    answerPointsCurve->attach(qwtPlot);
    answerCurve->attach(qwtPlot);

    connect(ui->openFile, &QAction::triggered, this, &Interface::openDataFile);
    connect(this, &Interface::fileIsNotGood, this, &Interface::fileIsNotCorrect);
    connect(ui->searchButton, &QPushButton::clicked, this, &Interface::fitLineRansacSlot);
    connect(qwtPlot, &CustomPlot::readyToGiveNewPoint, this, &Interface::onPlotMousePressEvent);
}

bool Interface::readDataFile(const QString& fileName) {
    clearAll();

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
    plotUpdate();

    file.close();
    return isFileGood;
}

void Interface::clearAll() {
    coordinates->clear();

    pointsCurve->setSamples(0);
    pointsCurve->attach(qwtPlot);

    answerPointsCurve->setSamples(0);
    answerPointsCurve->attach(qwtPlot);

    answerCurve->setSamples(0);
    answerCurve->attach(qwtPlot);
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

void Interface::plotUpdate() {
    QPolygonF newPoints;

    auto newPointsArray = coordinates->getPoints();

    for (size_t i = 0; i < newPointsArray.size(); ++i) {
        newPoints << QPointF(newPointsArray[i].x, newPointsArray[i].y);
    }

    pointsCurve->setSamples(newPoints);
    pointsCurve->attach(qwtPlot);
}

void Interface::fitLineRansacSlot(){
    auto pointsArray = coordinates->getPoints();
    auto [k, b, suitablePoints] = fitLineRansac(pointsArray);

    double minX = suitablePoints[0].x, maxX = suitablePoints[0].x;

    QPolygonF suitablePointsPolygon;

    for (size_t i = 0; i < suitablePoints.size(); ++i) {
        suitablePointsPolygon << QPointF(suitablePoints[i].x, suitablePoints[i].y);

        if (suitablePoints[i].x > maxX) maxX = suitablePoints[i].x;
        if (suitablePoints[i].x < minX) minX = suitablePoints[i].x;
    }

    answerPointsCurve->setSamples(suitablePointsPolygon);
    answerPointsCurve->attach(qwtPlot);

    QPolygonF answerCurvePoints;

    answerCurvePoints << QPointF(minX, k * minX + b) << QPointF(maxX, maxX * k + b);

    answerCurve->setSamples(answerCurvePoints);
    answerCurve->attach(qwtPlot);
}

void Interface::onPlotMousePressEvent() {
    coordinates->push_back(qwtPlot->getNewPoint());
    plotUpdate();
}

Interface::~Interface() {
    delete ui;

    delete coordinates;

    delete grid;
    delete magnifier;
    delete panner;
    delete picker;

    delete pointsCurve;
    delete answerPointsCurve;
    delete answerCurve;

    delete pointsSymbol;
    delete answerPointsSymbol;
    delete answerSymbol;
}

