#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <aboutwindow.h>
#include <technologieswindow.h>
#include <chpasswindow.h>
#include <manageaccountswindow.h>
#include <pch.h>

MainWindow::MainWindow(const QString& userName, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_userName(userName)
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);
    ui->statusbar->showMessage("User : " + m_userName);

    auto menuMyAccount = ui->menubar->addMenu("My account");

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

    connect(m_timer, &QTimer::timeout, this, &MainWindow::reAuthTimer);
    m_timer->start(TIME_INTERVAL);

    setWindowTitle("Main Window : " + userName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeMyPassword()
{
    PLOGI << "Main window : Change password (" << m_userName << ")";
    qDebug()<<"Change password";
    ChPassWindow chPassWindow(m_userName);
    chPassWindow.setModal(true);
    chPassWindow.exec();
}


void MainWindow::manageAccounts()
{
    PLOGI << "Main window : Manage accounts";
    qDebug()<<"Manage accounts";
    ManageAccountsWindow manageWindow;
    manageWindow.setModal(true);
    manageWindow.exec();
}

void MainWindow::aboutAuthor()
{
    qDebug()<<"About author";
    AboutWindow aboutWindow;
    aboutWindow.setModal(true);
    aboutWindow.exec();
}

void MainWindow::usedTechnologies()
{
    qDebug()<<"Used technologies";
    TechnologiesWindow techWindow;
    techWindow.setModal(true);
    techWindow.exec();
}

void MainWindow::reAuthTimer()
{
    qDebug() << "Re auth timer";
    m_timer->stop();

    m_timer->start();
}
