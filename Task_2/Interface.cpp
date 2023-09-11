#include "Interface.h"
#include "ui_Interface.h"

Interface::Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interface) {
    ui->setupUi(this);
}

Interface::~Interface() {
    delete ui;
}

