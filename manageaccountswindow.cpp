#include "manageaccountswindow.h"
#include "ui_manageaccountswindow.h"

ManageAccountsWindow::ManageAccountsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageAccountsWindow)
{
    ui->setupUi(this);
}

ManageAccountsWindow::~ManageAccountsWindow()
{
    delete ui;
}
