#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <aboutwindow.h>
#include <technologieswindow.h>
#include <chpasswindow.h>
#include <manageaccountswindow.h>
#include <questioncheckwindow.h>
#include <fileinfowindow.h>
#include <pch.h>
#include <manageusers.h>
#include <QFileDialog>
#include <QProgressDialog>
#include <QProgressBar>
#include <QThread>
#include <QInputDialog>

MainWindow::MainWindow(const QString& userName, bool activated, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_userName(userName)
    , m_timer(new QTimer(this))
    , m_activated(activated)
{
    ui->setupUi(this);
    ui->statusbar->showMessage("User : " + m_userName);

    ui->actionNew->setShortcut(Qt::CTRL + Qt::Key_N);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newFile);

    ui->actionOpen->setShortcut(Qt::CTRL + Qt::Key_O);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);

    ui->actionSave->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFile);

    ui->actionSave_as->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveAsFile);

    ui->actionParameters->setShortcut(Qt::CTRL + Qt::Key_I);
    connect(ui->actionParameters, &QAction::triggered, this, &MainWindow::parameters);

    ui->actionPrint->setShortcut(Qt::CTRL + Qt::Key_P);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::printFile);


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

    if(m_activated == false)
    {
        qDebug() << "Main Window : " + userName  + " doesn't have an activated account";
        PLOGI <<"Main Window : " + userName  + " doesn't have an activated account";
        ui->actionParameters->setDisabled(true);
        m_activateAction = ui->menubar->addAction("Activate");
        m_activateAction->setShortcut(Qt::CTRL + Qt::Key_A);
        connect(m_activateAction, &QAction::triggered, this, &MainWindow::activate);

    }
    else
    {
        qDebug() << "Main Window : " + userName  + " has an activated account";
        PLOGI <<"Main Window : " + userName  + " has an activated account";
    }

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

void MainWindow::activate()
{
    qDebug() << "Main window : " + m_userName + " tries to activate the account";
    PLOGI << "Main window : " + m_userName + " tries to activate the account";
    bool result;
    QString key = QInputDialog::getText(this, "Key", "Activation key : ", QLineEdit::Normal, "", &result);
    if(result)
    {
        qDebug() << "User enters key : " << key;
        PLOGI << "User " << m_userName << " accepts key window";
        if(ManageUsers::caesarCipher(key.toStdString(), CAESAR_CIPHER_SHIFT) == encryptedKey)
        {
            qDebug() << "The key is correct";
            PLOGI << "The key is correct. Activating account";
            ManageUsers::changeProperty(m_userName, ACTIVATED, true);
            ui->actionParameters->setDisabled(false);
            m_activateAction->deleteLater();
        }
        else
        {
            qDebug() << "Incorrect key";
            PLOGI << "Incorrect key";
            QMessageBox::warning(this, "Incorrect key", "This key is not correct");
        }
    }
    else
    {
        qDebug() << "User " << m_userName << " rejects key window";
        PLOGI << "User " << m_userName << " rejects key window";
    }
}

void MainWindow::openFile()
{
    qDebug() << "Main window : Open file";
    PLOGI <<    "Main window : Open file";
    if(ui->plainTextEdit->document()->isModified())
    {
        auto answer = QMessageBox::question(this, "Save file", "Do you want to save changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(answer == QMessageBox::Yes)
        {
            saveFile();
        }
        else if(answer == QMessageBox::No)
        {
            qDebug() << "Main window : Don't save file";
            PLOGI <<  "Main window : Don't save file";
        }
        else
        {
            qDebug() << "Main window : New file canceled";
            PLOGI << "Main window : New file canceled";
            return;
        }
    }

    QString filePath = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath());
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
            saveFile();
        }
        else if(answer == QMessageBox::No)
        {
            qDebug() << "Main window : Don't save file";
            PLOGI << "Main window : Don't save file";
        }
        else
        {
            qDebug() << "Main window : New file canceled";
            PLOGI << "Main window : New file canceled";
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
    if(!m_file.exists())
    {
        qDebug() << "You haven't opened any file yet";
        PLOGW << "You haven't opened any file yet";
        QMessageBox::warning(this, "No file", "You haven't opened any file yet");
        return;
    }
    FileInfoWindow fileInfoW(m_file.fileName());
    fileInfoW.setModal(true);
    fileInfoW.exec();
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
        PLOGI << "Questions check accepted";
    }
    else{
        qDebug() << "Questions check rejected";
        PLOGW << "Questions check rejected";
        QMessageBox::critical(this, "Fail", "Reauthentication failed");
        QApplication::quit();
        return;
    }

    m_timer->start();
}
