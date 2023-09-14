#include "Interface.h"
#include "ui_Interface.h"

Interface::Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interface) {
    ui->setupUi(this);

    setWindowTitle("RANSAC Line 2D");

    coordinates = new Coordinates;

    ui->qwtPlot->setCanvasBackground(Qt::white);
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

    picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, ui->qwtPlot->canvas());
    picker->setRubberBandPen(QColor(Qt::red));
    picker->setTrackerPen(QColor(Qt::black));
    picker->setStateMachine(new QwtPickerDragPointMachine());

    pointsCurve = new QwtPlotCurve;
    inlierPointsCurve = new QwtPlotCurve;
    outlierPointsCurve = new QwtPlotCurve;
    answerCurve = new QwtPlotCurve;

    pointsSymbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::gray), QPen(Qt::gray, 2), QSize(5, 5));
    inlierPointsSymbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::blue), QPen(Qt::blue, 2), QSize(5, 5));
    outlierPointsSymbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::red), QPen(Qt::red, 2), QSize(5, 5));
    answerSymbol = new QwtSymbol;

    pointsCurve->setSymbol(pointsSymbol);
    inlierPointsCurve->setSymbol(inlierPointsSymbol);
    outlierPointsCurve->setSymbol(outlierPointsSymbol);
    answerCurve->setSymbol(answerSymbol);

    pointsCurve->setStyle(QwtPlotCurve::NoCurve);
    pointsCurve->setLegendAttribute(QwtPlotCurve::LegendShowBrush, true);

    inlierPointsCurve->setStyle(QwtPlotCurve::NoCurve);
    inlierPointsCurve->setLegendAttribute(QwtPlotCurve::LegendShowBrush, true);

    outlierPointsCurve->setStyle(QwtPlotCurve::NoCurve);
    outlierPointsCurve->setLegendAttribute(QwtPlotCurve::LegendShowBrush, true);

    answerCurve->setPen(Qt::blue, 3);
    answerCurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    answerCurve->setLegendAttribute(QwtPlotCurve::LegendShowBrush, true);

    pointsCurve->attach(ui->qwtPlot);
    inlierPointsCurve->attach(ui->qwtPlot);
    outlierPointsCurve->attach(ui->qwtPlot);
    answerCurve->attach(ui->qwtPlot);

    connect(ui->openFile, &QAction::triggered, this, &Interface::openDataFile);
    connect(ui->saveInDataFile, &QAction::triggered, this, &Interface::saveInDataFile);
    connect(this, &Interface::fileIsNotGood, this, &Interface::fileIsNotCorrect);
    connect(ui->searchButton, &QPushButton::clicked, this, &Interface::fitLineRansacSlot);
    connect(ui->clearButton, &QPushButton::clicked, this, &Interface::clearAll);
    connect(picker, SIGNAL(appended(const QPoint&)), this, SLOT(clickOnPlot(const QPoint&)));
}

Interface::~Interface() {
    delete ui;

    delete coordinates;

    delete grid;
    delete magnifier;
    delete panner;
    delete picker;

    delete pointsCurve;
    delete inlierPointsCurve;
    delete answerCurve;

    delete pointsSymbol;
    delete inlierPointsSymbol;
    delete answerSymbol;
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

    plotUpdate();
    file.close();

    return isFileGood;
}

void Interface::clearAll() {
    coordinates->clear();

    pointsCurve->setSamples(0);
    inlierPointsCurve->setSamples(0);
    outlierPointsCurve->setSamples(0);
    answerCurve->setSamples(0);

    pointsCurve->attach(ui->qwtPlot);
    inlierPointsCurve->attach(ui->qwtPlot);
    outlierPointsCurve->attach(ui->qwtPlot);
    answerCurve->attach(ui->qwtPlot);
}

void Interface::openDataFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "", "Текстовые файлы (*.txt)");

    if (!readDataFile(fileName)) {
        emit fileIsNotGood();
    }
}

void Interface::saveInDataFile() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить в файл"), "", tr("Текстовые файлы (*.txt)"));
    if(fileName.isEmpty()) return;

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);

        auto points = coordinates->getPoints();

        for (size_t i = 0; i < coordinates->size(); ++i) {
            out << "(" << QString::number(points[i].x) << ", " << QString::number(points[i].y) << ")\n";
        }

        file.close();
    }

    plotUpdate();
}

void Interface::fileIsNotCorrect() {
    QMessageBox::warning(this, "Ошибка", "При открытии файла произошла ошибка. Он не был выбран или его невозможно прочитать");
    clearAll();
}

void Interface::plotUpdate() {
    QPolygonF newPoints;
    auto newPointsArray = coordinates->getPoints();

    for (size_t i = 0; i < newPointsArray.size(); ++i) {
        newPoints << QPointF(newPointsArray[i].x, newPointsArray[i].y);
    }

    pointsCurve->setSamples(newPoints);
    pointsCurve->attach(ui->qwtPlot);
}

void Interface::fitLineRansacSlot(){
    try {
        QElapsedTimer timer;
        timer.start();

        auto [k, b, inlierPoints, outlierPoints] = coordinates->fitLineRansac();
        double minX = inlierPoints[0].x, maxX = inlierPoints[0].x;

        QPolygonF inlierPointsPolygon, outlierPointsPolygon, answerCurvePoints;

        for (size_t i = 0; i < inlierPoints.size(); ++i) {
            inlierPointsPolygon << QPointF(inlierPoints[i].x, inlierPoints[i].y);

            if (inlierPoints[i].x > maxX) maxX = inlierPoints[i].x;
            if (inlierPoints[i].x < minX) minX = inlierPoints[i].x;
        }

        for (size_t i = 0; i < outlierPoints.size(); ++i) {
            outlierPointsPolygon << QPointF(outlierPoints[i].x, outlierPoints[i].y);
        }

        answerCurvePoints << QPointF(minX, k * minX + b) << QPointF(maxX, maxX * k + b);

        inlierPointsCurve->setSamples(inlierPointsPolygon);
        outlierPointsCurve->setSamples(outlierPointsPolygon);
        answerCurve->setSamples(answerCurvePoints);

        inlierPointsCurve->attach(ui->qwtPlot);
        outlierPointsCurve->attach(ui->qwtPlot);
        answerCurve->attach(ui->qwtPlot);

        ui->equationOutLine->setText(QString("y = ") + QString::number(k, 'f', 1) + ((b >= 0) ? (QString(" * x + ") + QString::number(b, 'f', 1)) : QString(" * x - ") + QString::number(-b, 'f', 1)));
        ui->timeLabel->setText(QString("Время (мс): ") + QString::number(timer.elapsed()));
    }
    catch (std::runtime_error& error) {
        clearAll();
        plotUpdate();
    }
}

void Interface::clickOnPlot(const QPoint& pos) {
    double x = ui->qwtPlot->invTransform(QwtPlot::xBottom, pos.x());
    double y = ui->qwtPlot->invTransform(QwtPlot::yLeft, pos.y());

    coordinates->push_back(Point(x, y));
    plotUpdate();
}
