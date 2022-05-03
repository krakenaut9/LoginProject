#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <pch.h>
#include "plog/Initializers/RollingFileInitializer.h"
#include <questioncheckwindow.h>
#include <manageusers.h>
#include <captchawindow.h>

int main(int argc, char *argv[])
{
    plog::init(plog::info, "LogFile.log");
    PLOGI << "\nProgram started";

    if(ManageUsers::initUsersFile(USERS_FILE) != QFile::NoError)
    {
        qDebug("Init users file failed");
        PLOGF << "Init users file failed";
        return 2;
    }

    QApplication a(argc, argv);
    LoginWindow lw;
    lw.show();
    CaptchaWindow cw;
    if(cw.exec() == QDialog::Accepted)
    {
        qDebug() << "User passes captcha test successfully";
        PLOGI << "User passes captcha test successfully";
    }
    else
    {
        qDebug() << "Captcha test rejected";
        PLOGI << "Captcha test rejected";
        return 3;
    }
    if(lw.exec() == QDialog::Accepted)
    {
        if(lw.GetFirstLogin())
        {
            qDebug() << "User Logged in for the first time";
            PLOGI << "User " << lw.GetUserName() << " adds answers to the secret questions";
            QuestionCheckWindow Questions(lw.GetUserName(), ADD_ANSWERS);
            if(Questions.exec() == QDialog::Accepted)
            {
                PLOGI << "Questions window accepted";
                qDebug() << "Questions window accepted";
            }
            else
            {
                qDebug() << "Questions window rejected";
                PLOGE << "Questions window rejected";
                return 4;
            }
        }

        PLOGI << "User : " << lw.GetUserName() << " logged in successfully";
        qDebug()<<"Login and password are correct";
        MainWindow w(lw.GetUserName(), lw.GetActivated());
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
