#include "Interface.h"
#include "ui_Interface.h"

Interface::Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interface) {
    ui->setupUi(this);

    setWindowTitle("Server");

    ui->xSpinBox->setRange(0, 63);
    ui->ySpinBox->setRange(-32, 31);
    ui->vSpinBox->setRange(0, 255);
    ui->mSpinBox->setRange(0, 3);
    ui->sSpinBox->setRange(0, 3);
    ui->aDoubleSpinBox->setRange(-12.8, 12.7);
    ui->pSpinBox->setRange(0, 130);

    ui->rSpinBox_1->setRange(0, 1);
    ui->rSpinBox_2->setRange(0, 1);
    ui->rSpinBox_3->setRange(0, 1);
    ui->rSpinBox_4->setRange(0, 1);

    ui->rSpinBox_5->setRange(0, 1);
    ui->rSpinBox_6->setRange(0, 1);
    ui->rSpinBox_7->setRange(0, 1);
    ui->rSpinBox_8->setRange(0, 1);

    ui->rSpinBox_9->setRange(0, 1);
    ui->rSpinBox_10->setRange(0, 1);
    ui->rSpinBox_11->setRange(0, 1);
    ui->rSpinBox_12->setRange(0, 1);

    ui->rSpinBox_13->setRange(0, 1);
    ui->rSpinBox_14->setRange(0, 1);
    ui->rSpinBox_15->setRange(0, 1);
    ui->rSpinBox_16->setRange(0, 1);

    ui->ipLineEdit->setText("127.0.0.1");

    bitManipulator = new BM::BitManipulator;
    socketManager = new SM::SocketManager;

    connect(ui->homePortLineEdit, SIGNAL(returnPressed()), this, SLOT(slotBindPort()));
    connect(socketManager, SIGNAL(signalDisconnectedHost()), this, SLOT(slotDisconnectedHost()));
    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(slotSend()));
    connect(socketManager, SIGNAL(arrivalDatagram()), this, SLOT(slotArrivalDatagram()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(slotClear()));
}

Interface::~Interface() {
    delete ui;

    delete bitManipulator;
    delete socketManager;
}

void Interface::slotClear() {
    ui->xSpinBox->setValue(0);
    ui->ySpinBox->setValue(0);
    ui->vSpinBox->setValue(0);
    ui->mSpinBox->setValue(0);
    ui->sSpinBox->setValue(0);
    ui->aDoubleSpinBox->setValue(0);
    ui->pSpinBox->setValue(0);

    ui->rSpinBox_1->setValue(0);
    ui->rSpinBox_2->setValue(0);
    ui->rSpinBox_3->setValue(0);
    ui->rSpinBox_4->setValue(0);

    ui->rSpinBox_5->setValue(0);
    ui->rSpinBox_6->setValue(0);
    ui->rSpinBox_7->setValue(0);
    ui->rSpinBox_8->setValue(0);

    ui->rSpinBox_9->setValue(0);
    ui->rSpinBox_10->setValue(0);
    ui->rSpinBox_11->setValue(0);
    ui->rSpinBox_12->setValue(0);

    ui->rSpinBox_13->setValue(0);
    ui->rSpinBox_14->setValue(0);
    ui->rSpinBox_15->setValue(0);
    ui->rSpinBox_16->setValue(0);
}

void Interface::slotBindPort() {
    socketManager->setHostNPort(ui->homePortLineEdit->text().toInt());
    socketManager->slotBindPort();
}

void Interface::slotDisconnectedHost() {
    ui->homePortLineEdit->setText("Порт не доступен!");
}

void Interface::slotSend() {
    int r = 0;

    r |= ui->rSpinBox_1->value() << 15;
    r |= ui->rSpinBox_2->value() << 14;
    r |= ui->rSpinBox_3->value() << 13;
    r |= ui->rSpinBox_4->value() << 12;

    r |= ui->rSpinBox_5->value() << 11;
    r |= ui->rSpinBox_6->value() << 10;
    r |= ui->rSpinBox_7->value() << 9;
    r |= ui->rSpinBox_8->value() << 8;

    r |= ui->rSpinBox_9->value() << 7;
    r |= ui->rSpinBox_10->value() << 6;
    r |= ui->rSpinBox_11->value() << 5;
    r |= ui->rSpinBox_12->value() << 4;

    r |= ui->rSpinBox_13->value() << 3;
    r |= ui->rSpinBox_14->value() << 2;
    r |= ui->rSpinBox_15->value() << 1;
    r |= ui->rSpinBox_16->value();

    bitManipulator->numsToBitsFirstWord(ui->xSpinBox->value(), ui->ySpinBox->value());
    bitManipulator->numsToBitsSecondWord(ui->vSpinBox->value(), ui->mSpinBox->value(), ui->sSpinBox->value());
    bitManipulator->numsToBitsThirdWord(ui->aDoubleSpinBox->value(), ui->pSpinBox->value());
    bitManipulator->numsToBitsFourthWord(r);

    socketManager->sendDatagram(ui->ipLineEdit->text(), ui->portLineEdit->text().toInt(), bitManipulator->getPacket());
}

void Interface::slotArrivalDatagram() {
    qDebug() << "slotArrivalDatagram()";
}
