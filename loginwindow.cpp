#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <pch.h>
LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setFixedSize(550, 400);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked()
{
    QString userName = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    accept();
    SetUserName(userName);

    if(userName.length() == 0)
    {
        qDebug()<<"Empty username";
        ui->usernameLineEdit->setFocus();
    }
    if(userName == "admin" && password == "123")
    {
        SetUserName(userName);
        accept();
    }
}


void LoginWindow::on_showButton_pressed()
{
    ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
}


void LoginWindow::on_showButton_released()
{
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}

QString LoginWindow::GetUserName()const noexcept
{
    return m_userName;
}
void LoginWindow::SetUserName(const QString& userName)
{
    m_userName = userName;
}
