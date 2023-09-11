#pragma once

#include <QMainWindow>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Interface; }
QT_END_NAMESPACE

class Interface : public QMainWindow {
    Q_OBJECT
public:
    Interface(QWidget *parent = nullptr);
    ~Interface();
private:
    Ui::Interface *ui;
};
