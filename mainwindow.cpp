#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <pch.h>
MainWindow::MainWindow(const QString& userName, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_userName(userName)
{
    ui->setupUi(this);
    ui->statusbar->showMessage("User : " + m_userName);

    auto MyAccount = ui->menubar->addMenu("My account");

    auto actionShowInformation = MyAccount->addAction("Show information");
    connect(actionShowInformation, &QAction::triggered, this, &MainWindow::showAccountInformation);
    auto actionChangePassword = MyAccount->addAction("Change password");
    connect(actionChangePassword, &QAction::triggered, this, &MainWindow::changeMyPassword);

    MyAccount->addSeparator();

    auto actionLogOut = MyAccount->addAction("Log out");
    connect(actionLogOut, &QAction::triggered, this, &QApplication::quit);

    if(m_userName == "admin")
    {
        //Admin can manage profiles
        auto actionManageAccounts = ui->menubar->addAction("Manage accounts");
        connect(actionManageAccounts, &QAction::triggered, this, &MainWindow::manageAccounts);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeMyPassword()
{
    qDebug()<<"Change password";
}

void MainWindow::showAccountInformation()
{
    qDebug()<<"Show information";
}

void MainWindow::manageAccounts()
{
    qDebug()<<"Manage accounts";
}

