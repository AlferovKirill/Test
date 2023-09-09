#include "SocketManager.h"

SM::SocketManager::SocketManager() {
    udpSocket = new QUdpSocket;
}

SM::SocketManager::~SocketManager() {
    udpSocket->disconnectFromHost();
    delete udpSocket;
}

void SM::SocketManager::initSocket() {
    if (udpSocket->state() != QAbstractSocket::UnconnectedState) {
        udpSocket->disconnectFromHost();
    }

    if (!udpSocket->bind(port)) throw "Disconnected";

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(slotReadPendingDatagrams()));
}

void SM::SocketManager::setHostNPort(int _port) {
    port = _port;
}

void SM::SocketManager::sendDatagram(const QString& destinationHost, int destinationPort, BM::Packet packet) {
    QByteArray datagramParametrs(reinterpret_cast<char*>(&packet), sizeof(packet));
    udpSocket->writeDatagram(datagramParametrs, QHostAddress(destinationHost), destinationPort);
    QThread::msleep(5);
}

BM::Packet SM::SocketManager::getArrivalPacket() {
    return packet;
}

void SM::SocketManager::slotReadPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;

        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        bool isNeedPacket = (datagram.size() == sizeof(BM::Packet)) && (reinterpret_cast<BM::Packet*>(datagram.data())->name == static_cast<char>(0xAA));

        if (isNeedPacket) {
            packet = *reinterpret_cast<BM::Packet*>(datagram.data());

            emit arrivalDatagram();
        }
    }
}

void SM::SocketManager::slotBindPort() {
    try {
        initSocket();
    }
    catch(...) {
        emit signalDisconnectedHost();
    }
}
