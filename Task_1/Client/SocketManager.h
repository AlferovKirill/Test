#pragma once

#include <QWidget>
#include <QDebug>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QThread>

#include "BitManipulator.h"

namespace SM {
    class SocketManager : public QWidget {
        Q_OBJECT
    public:
        SocketManager();
        ~SocketManager();
        void initSocket();
        void setHostNPort(int _port);
        void sendDatagram(const QString& destinationHost, int destinationPort, BM::Packet packet);
        BM::Packet getArrivalPacket();
    public slots:
        void slotReadPendingDatagrams();
        void slotBindPort();
    signals:
        void signalDisconnectedHost();
        void arrivalDatagram();
    private:
        QUdpSocket* udpSocket;
        int port;

        BM::Packet packet;
    };
}
