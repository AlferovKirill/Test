#include "Interface.h"
#include "ui_Interface.h"

Interface::Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interface) {
    ui->setupUi(this);

    setWindowTitle("Client");

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

    bitManipulator = new BM::BitManipulator;
    socketManager = new SM::SocketManager;

    connect(ui->homePortLineEdit, SIGNAL(returnPressed()), this, SLOT(slotBindPort()));
    connect(socketManager, SIGNAL(signalDisconnectedHost()), this, SLOT(slotDisconnectedHost()));
    connect(socketManager, SIGNAL(arrivalDatagram()), this, SLOT(slotArrivalDatagram()));
}

Interface::~Interface() {
    delete ui;

    delete bitManipulator;
    delete socketManager;
}

void Interface::slotUpdate() {
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

void Interface::slotArrivalDatagram() {
    bitManipulator->insertPacket(socketManager->getArrivalPacket());

    int x, y, v, m, s, p, r;
    float a;

    bitManipulator->bitsToNumsFirstWord(x, y);
    bitManipulator->bitsToNumsSecondWord(v, m, s);
    bitManipulator->bitsToNumsThirdWord(a, p);
    bitManipulator->bitsToNumsFourthWord(r);

    ui->xSpinBox->setValue(x);
    ui->ySpinBox->setValue(y);
    ui->vSpinBox->setValue(v);
    ui->mSpinBox->setValue(m);
    ui->sSpinBox->setValue(s);
    ui->aDoubleSpinBox->setValue(a);
    ui->pSpinBox->setValue(p);

    ui->rSpinBox_1->setValue(r & (1 << 15));
    ui->rSpinBox_2->setValue(r & (1 << 14));
    ui->rSpinBox_3->setValue(r & (1 << 13));
    ui->rSpinBox_4->setValue(r & (1 << 12));

    ui->rSpinBox_5->setValue(r & (1 << 11));
    ui->rSpinBox_6->setValue(r & (1 << 10));
    ui->rSpinBox_7->setValue(r & (1 << 9));
    ui->rSpinBox_8->setValue(r & (1 << 8));

    ui->rSpinBox_9->setValue(r & (1 << 7));
    ui->rSpinBox_10->setValue(r & (1 << 6));
    ui->rSpinBox_11->setValue(r & (1 << 5));
    ui->rSpinBox_12->setValue(r & (1 << 4));

    ui->rSpinBox_13->setValue(r & (1 << 3));
    ui->rSpinBox_14->setValue(r & (1 << 2));
    ui->rSpinBox_15->setValue(r & (1 << 1));
    ui->rSpinBox_16->setValue(r & 1);
}
