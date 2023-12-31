#pragma once

#include <QMainWindow>

#include "BitManipulator.h"
#include "SocketManager.h"

namespace Ui {
    class Interface;
}

class Interface : public QMainWindow {
    Q_OBJECT
public:
    explicit Interface(QWidget *parent = nullptr);
    ~Interface();
public slots:
    void slotClear();
    void slotBindPort();
    void slotDisconnectedHost();
    void slotSend();
private:
    Ui::Interface *ui;

    BM::BitManipulator* bitManipulator;
    SM::SocketManager* socketManager;
};
