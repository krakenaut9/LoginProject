#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <aboutwindow.h>
#include <technologieswindow.h>
#include <chpasswindow.h>
#include <manageaccountswindow.h>
#include <questioncheckwindow.h>
#include <pch.h>
#include <QFileDialog>
#include <QProgressDialog>
#include <QProgressBar>
#include <QThread>

MainWindow::MainWindow(const QString& userName, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_userName(userName)
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);
    ui->statusbar->showMessage("User : " + m_userName);
    //setCentralWidget(ui->plainTextEdit);
    auto menuFile = ui->menubar->addMenu("File");

    auto actionNewFile = menuFile->addAction("New file");
    actionNewFile->setShortcut(Qt::CTRL + Qt::Key_N);
    connect(actionNewFile, &QAction::triggered, this, &MainWindow::newFile);

    auto actionOpenFile = menuFile->addAction("Open file");
    actionOpenFile->setShortcut(Qt::CTRL + Qt::Key_O);
    connect(actionOpenFile, &QAction::triggered, this, &MainWindow::openFile);

    auto actionSaveFile = menuFile->addAction("Save");
    actionSaveFile->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(actionSaveFile, &QAction::triggered, this, &MainWindow::saveFile);

    auto actionSaveAsFile = menuFile->addAction("Save as");
    actionSaveAsFile->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    connect(actionSaveAsFile, &QAction::triggered, this, &MainWindow::saveAsFile);

    auto actionFileParameters = menuFile->addAction("Paremeters");
    actionFileParameters->setShortcut(Qt::CTRL + Qt::Key_I);
    connect(actionFileParameters, &QAction::triggered, this, &MainWindow::parameters);

    auto actionPrintFile = menuFile->addAction("Print");
    actionPrintFile->setShortcut(Qt::CTRL + Qt::Key_P);
    connect(actionPrintFile, &QAction::triggered, this, &MainWindow::printFile);


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
        actionManageAccounts->setShortcut(Qt::CTRL + Qt::Key_M);
        connect(actionManageAccounts, &QAction::triggered, this, &MainWindow::manageAccounts);
    }

    auto menuAbout = ui->menubar->addMenu("About");

    auto actionAboutAuthor = menuAbout->addAction("About author");
    connect(actionAboutAuthor, &QAction::triggered, this, &MainWindow::aboutAuthor);
    auto actionUsedTechnologies = menuAbout->addAction("Used technologies");
    connect(actionUsedTechnologies, &QAction::triggered, this, &MainWindow::usedTechnologies);

    connect(m_timer, &QTimer::timeout, this, &MainWindow::reAuthTimer);
    m_timer->start(TIME_INTERVAL);

    setWindowTitle("Main Window : " + userName  + " (" + UNTITLED + ')');
}

MainWindow::~MainWindow()
{
    delete ui;
    PLOGI << "Program finished : " << m_userName;
}

void MainWindow::changeMyPassword()
{
    PLOGI << "Main window : " << m_userName << " wants to change their password ";
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

void MainWindow::openFile()
{
    qDebug() << "Main window : Open file";
    PLOGI <<    "Main window : Open file";
    QString filePath = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath());
    if(ui->plainTextEdit->document()->isModified())
    {
        auto answer = QMessageBox::question(this, "Save file", "Do you want to save changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(answer == QMessageBox::Yes)
        {
            //TODO : Save file
            saveFile();
        }
        else if(answer == QMessageBox::No)
        {
            qDebug() << "Main window : Don't save file";
        }
        else
        {
            qDebug() << "Main window : New file canceled";
            return;
        }
    }
    if(!filePath.isEmpty())
    {
        qDebug() << "Main window : " << m_userName << " tries to open file : " << filePath;
        PLOGI << "Main window : " << m_userName << " tries to open file : " << filePath;
        m_file.setFileName(filePath);
        if(!m_file.open(QFile::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly))
        {
            qDebug() << "Main window : Unable to open file " << m_file.fileName();
            PLOGE << "Main window : Unable to open file " << m_file.fileName();
            QMessageBox::critical(this, "Open failure", "Unable to open file" + m_file.fileName());
            return;
        }
        ui->plainTextEdit->setPlainText(m_file.readAll());
        ui->plainTextEdit->moveCursor(QTextCursor::End);
        setWindowTitle("Main Window : " + m_userName  + " (" + QFileInfo(filePath).fileName() + ')');
    }
    else
    {
        qDebug() << "Main window : File open rejected";
        PLOGI << "Main window : File open rejected";
    }

}

void MainWindow::newFile()
{
    qDebug() << "Main window : New file";
    PLOGI <<    "Main window : New file";
    if(ui->plainTextEdit->document()->isModified())
    {
        auto answer = QMessageBox::question(this, "Save file", "Do you want to save changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(answer == QMessageBox::Yes)
        {
            //TODO : Save file
            saveFile();
        }
        else if(answer == QMessageBox::No)
        {
            qDebug() << "Main window : Don't save file";
        }
        else
        {
            qDebug() << "Main window : New file canceled";
            return;
        }
    }
    ui->plainTextEdit->clear();
    m_file.close();
    m_file.setFileName("");
    setWindowTitle("Main Window : " + m_userName  + " (" + UNTITLED + ')');
}

void MainWindow::saveFile()
{
    qDebug() << "Main window : Save file";
    PLOGI <<    "Main window : Save file";
    if(!m_file.exists())
    {
        qDebug() << "Main window : File doesn't exist";
        PLOGI << "Main window : File doesn't exist";
        saveAsFile();
        return;
    }
    m_file.flush();
    m_file.close();
    if(!m_file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate))
    {
        qDebug() << "Main window : Unable to open file " << m_file.fileName();
        PLOGE << "Main window : Unable to open file " << m_file.fileName();
        QMessageBox::critical(this, "Open failure", "Unable to open file" + m_file.fileName());
        return;
    }
    QTextStream fileStream(&m_file);
    fileStream << ui->plainTextEdit->toPlainText();
    ui->plainTextEdit->document()->setModified(false);
}

void MainWindow::saveAsFile()
{
    qDebug() << "Main window : Save as file";
    PLOGI <<    "Main window : Save as file";
    auto newFilePath = QFileDialog::getSaveFileName(this, "Save as", QDir::currentPath(), "Text files (*.txt)");
    if(newFilePath.isEmpty())
    {
        qDebug() << "Save as window rejected";
        PLOGI << "Save as window rejected";
        return;
    }
    qDebug() << "Main window : Save as path " << newFilePath;
    if(m_file.isOpen())
    {
        m_file.flush();
        m_file.close();
    }
    m_file.setFileName(newFilePath);
    if(!m_file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate))
    {
        qDebug() << "Main window : Unable to open file " << m_file.fileName();
        PLOGE << "Main window : Unable to open file " << m_file.fileName();
        QMessageBox::critical(this, "Open failure", "Unable to open file" + m_file.fileName());
        return;
    }
    QTextStream fileStream(&m_file);
    fileStream << ui->plainTextEdit->toPlainText();
    ui->plainTextEdit->document()->setModified(false);
    setWindowTitle("Main Window : " + m_userName  + " (" + QFileInfo(newFilePath).fileName() + ')');
}

void MainWindow::parameters()
{
    qDebug() << "Main window : File parameters";
    PLOGI <<    "Main window : File parameters";
}
void MainWindow::printFile()
{
    qDebug() << "Main window : Print file";
    PLOGI <<    "Main window : Print file";
    QProgressDialog pd(this);
    pd.setRange(0,100);
    pd.setLabelText("Printing the text");
    pd.setWindowTitle("Printing");
    pd.setFixedSize(400, 100);
    QProgressBar pb;
    pd.setBar(&pb);
    pd.setModal(true);
    pd.setCancelButton(nullptr);
    pd.show();
    int progress = 0;

    for(int i=0;i<100; ++i)
    {
        QThread::msleep(30);
        QApplication::processEvents();
        ++progress;
        pd.setValue(progress);
    }
    pd.deleteLater();
}

void MainWindow::manageAccounts()
{
    PLOGI << "Main window : Manage accounts window called";
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
