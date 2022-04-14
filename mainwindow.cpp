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

    auto menuMyAccount = ui->menubar->addMenu("My account");

    auto actionShowInformation = menuMyAccount->addAction("Show information");
    connect(actionShowInformation, &QAction::triggered, this, &MainWindow::showAccountInformation);
    auto actionChangePassword = menuMyAccount->addAction("Change password");
    connect(actionChangePassword, &QAction::triggered, this, &MainWindow::changeMyPassword);

    menuMyAccount->addSeparator();

    auto actionLogOut = menuMyAccount->addAction("Log out");
    connect(actionLogOut, &QAction::triggered, this, &QApplication::quit);

    if(m_userName == ADMIN)
    {
        //Admin can manage profiles
        auto actionManageAccounts = ui->menubar->addAction("Manage accounts");
        connect(actionManageAccounts, &QAction::triggered, this, &MainWindow::manageAccounts);
    }

    auto menuAbout = ui->menubar->addMenu("About");

    auto actionAboutAuthor = menuAbout->addAction("About author");
    connect(actionAboutAuthor, &QAction::triggered, this, &MainWindow::aboutAuthor);
    auto actionUsedTechnologies = menuAbout->addAction("Used technologies");
    connect(actionUsedTechnologies, &QAction::triggered, this, &MainWindow::usedTechnologies);
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

void MainWindow::aboutAuthor()
{
    qDebug()<<"About author";
}

void MainWindow::usedTechnologies()
{
    qDebug()<<"Used technologies";
}

