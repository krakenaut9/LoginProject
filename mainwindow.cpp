#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <aboutwindow.h>
#include <technologieswindow.h>
#include <chpasswindow.h>
#include <manageaccountswindow.h>
#include <questioncheckwindow.h>
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
    PLOGI << "Program finished : " << m_userName;
}

void MainWindow::changeMyPassword()
{
    PLOGI << "Main window : Change password (" << m_userName << ")";
    qDebug()<<"Change password";
    ChPassWindow chPassWindow(m_userName);
    chPassWindow.setModal(true);
    if(chPassWindow.exec() == QDialog::Accepted)
    {
        PLOGI << "Password successfully changed";
    }
    else
    {
        PLOGW << "Change password window rejected";
    }
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
    PLOGI << m_userName << " reads about author";
}

void MainWindow::usedTechnologies()
{
    qDebug()<<"Used technologies";
    TechnologiesWindow techWindow;
    techWindow.setModal(true);
    techWindow.exec();
    PLOGI << m_userName << " reads about used technologies";
}

void MainWindow::reAuthTimer()
{
    PLOGI << "Reauthentication time";
    qDebug() << "Re auth timer";
    m_timer->stop();

    PLOGI << "Reauthentication : Questions";
    qDebug() << "Questions";

    QuestionCheckWindow questionsCheck(m_userName, ANSWER_THE_QUESTIONS);
    if(questionsCheck.exec() == QDialog::Accepted)
    {
        qDebug() << "Questions check accepted";

    }
    else{
        qDebug()<< "Questions check rejected";
        QMessageBox::critical(this, "Fail", "Reauthentication failed");
        QApplication::quit();
        return;
    }

    m_timer->start();
}
