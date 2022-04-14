#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    ui->textLabel->setTextFormat(Qt::RichText);
    ui->textLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->textLabel->setOpenExternalLinks(true);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}
