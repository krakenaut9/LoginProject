#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <pch.h>
#include "plog/Initializers/RollingFileInitializer.h"

int main(int argc, char *argv[])
{
    plog::init(plog::info, "LogFile.log");
    PLOGI << "Program started";

    if(ManageUsers::initUsersFile(USERS_FILE) != QFile::NoError)
    {
        qDebug("Init users file failed");
        PLOGF << "Init users file failed";
        return 2;
    }

    if(ManageUsers::initQuestionsFile(QUESTIONS_FILE) != QFile::NoError)
    {
        qDebug("Init questions file failed");
        PLOGF << "Init questions file failed";
        return 3;
    }

    QApplication a(argc, argv);
    LoginWindow lw;
    if(lw.exec() == QDialog::Accepted)
    {
        PLOGI << "User : " << lw.GetUserName() << " logged in successfully";
        qDebug()<<"Login and password are correct";
        MainWindow w(lw.GetUserName());
        w.show();
        return a.exec();
    }
    else
    {
        PLOGW << "Login window rejected";
        qDebug()<<"Login window rejected";
        return 1;
    }
    return 0;
}
