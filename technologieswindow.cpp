#include "technologieswindow.h"
#include "ui_technologieswindow.h"
#include <pch.h>
TechnologiesWindow::TechnologiesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TechnologiesWindow)
{
    ui->setupUi(this);
    ui->aboutQTButton->setDefault(false);
    //ui->exitButton->setDefault(true);
    ui->exitButton->setFocus();
    connect(ui->aboutQTButton, &QPushButton::clicked, this, &TechnologiesWindow::aboutQT);
}

TechnologiesWindow::~TechnologiesWindow()
{
    delete ui;
}

void TechnologiesWindow::aboutQT()
{
    QMessageBox::aboutQt(this, "QT Framework");
    ui->exitButton->setFocus();
}
