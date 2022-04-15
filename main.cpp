#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <pch.h>

int main(int argc, char *argv[])
{
    if(!QFile(USERS_FILE).exists())
    {
        QFile usersFile(USERS_FILE);
        if(!usersFile.open(QIODevice::NewOnly | QIODevice::ReadWrite | QIODevice::Text))
        {
           qDebug()<<"New file creation failed";
           return QFile::FatalError;
        }
        QJsonObject mainObject;
        QJsonObject adminUser;
        adminUser.insert(PASSWORD, "");
        adminUser.insert(IS_BLOCKED, false);
        adminUser.insert(RESTRICTED_PASSWORD, false);
        adminUser.insert(FIRST_LOGIN, true);
        mainObject.insert(ADMIN, adminUser);
        QJsonDocument jsonDoc(mainObject);
        qDebug() << jsonDoc.toJson(QJsonDocument::Indented);
        usersFile.write(jsonDoc.toJson(QJsonDocument::Indented));
        usersFile.close();
    }
    QApplication a(argc, argv);
    LoginWindow lw;
    if(lw.exec() == QDialog::Accepted)
    {
        qDebug()<<"Login and password are correct";
        MainWindow w(lw.GetUserName());
        w.show();
        return a.exec();
    }
    else
    {
        qDebug()<<"Incorrect Login and password";
        return 1;
    }
    return 0;
}
