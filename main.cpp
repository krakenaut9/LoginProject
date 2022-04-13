#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
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
