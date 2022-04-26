#include "chpasswindow.h"
#include "ui_chpasswindow.h"
#include <pch.h>
#include <manageusers.h>

ChPassWindow::ChPassWindow(QString userName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChPassWindow),
    m_userName(userName)
{
    QFile jsonFile(USERS_FILE);
    ui->setupUi(this);
    this->setFixedSize(550, 400);

    if(!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly))
    {
        PLOGE << "Change password window : Failed to open json file";
        qDebug() << "Change password window : Failed to open json file";
        return;
    }
    QJsonParseError JsonParseError;
    m_jsonDocument = QJsonDocument::fromJson(jsonFile.readAll(), &JsonParseError);

    connect(ui->changePassButton, &QPushButton::clicked, this, &ChPassWindow::changeButtonClicked);


}

ChPassWindow::~ChPassWindow()
{
    delete ui;
}


void ChPassWindow::changeButtonClicked()
{
    PLOGI << "Change password window : User "<< m_userName << " tries to change the password";
    qDebug() << "Change button clicked";
    QJsonObject RootObject;

    RootObject = m_jsonDocument.object();
    QJsonObject::iterator it = RootObject.find(m_userName);
    if(it == RootObject.end())
    {
        PLOGE << "Change password window : No such user";
        qDebug() << "Change password window : No such user";
        ui->informLabel->setText("No such user");
        return;
    }

    if(it.value().toObject()[PASSWORD] != ui->oldPassLineEdit->text())
    {
        PLOGW << "Change password window : Incorrect password";
        qDebug() << "Change password window : Incorrect password";
        ui->informLabel->setText("Incorrect password");
        ui->oldPassLineEdit->clear();
        ui->oldPassLineEdit->setFocus();
        return;
    }

    QString firstNewPassword = ui->firstNewPassLineEdit->text();
    QString secondNewPassword = ui->secondNewPassLineEdit->text();

    if(firstNewPassword.isEmpty() || secondNewPassword.isEmpty())
    {
        qDebug() << "Change password window : empty password";
        ui->informLabel->setText("Empty passwords are not allowed");
        if(firstNewPassword.isEmpty())
        {
            ui->firstNewPassLineEdit->setFocus();
        }
        else
        {
            ui->secondNewPassLineEdit->setFocus();
        }
        return;
    }

    if(firstNewPassword != secondNewPassword)
    {
        qDebug() << "Change password window : Passwords are not equal";
        ui->informLabel->setText("Passwords are not equal");
        ui->firstNewPassLineEdit->clear();
        ui->secondNewPassLineEdit->clear();
        ui->firstNewPassLineEdit->setFocus();
        return;
    }
    if(it.value().toObject()[RESTRICTED_PASSWORD].toBool())
    {
        if(!ManageUsers::isPasswordValid(firstNewPassword))
        {
            PLOGW << "Change password window : Incorrect new password";
            qDebug() << "Incorrect new password";
            ui->informLabel->setText("Incorrect new password. The password must contain numbers and symbols +-*/=%()^:");
            ui->firstNewPassLineEdit->setFocus();
            return;
        }
    }

    ManageUsers::changeProperty(m_userName, PASSWORD, firstNewPassword);
    PLOGI << "Change password window : New password accepted";
    accept();

}

void ChPassWindow::on_showOldPassButton_pressed()
{
    ui->oldPassLineEdit->setEchoMode(QLineEdit::Normal);
}


void ChPassWindow::on_showOldPassButton_released()
{
    ui->oldPassLineEdit->setEchoMode(QLineEdit::Password);
    ui->oldPassLineEdit->setFocus();
}


void ChPassWindow::on_showFirstNewPassButton_pressed()
{
    ui->firstNewPassLineEdit->setEchoMode(QLineEdit::Normal);
}


void ChPassWindow::on_showFirstNewPassButton_released()
{
    ui->firstNewPassLineEdit->setEchoMode(QLineEdit::Password);
    ui->firstNewPassLineEdit->setFocus();
}


void ChPassWindow::on_showSecondNewPassButton_pressed()
{
    ui->secondNewPassLineEdit->setEchoMode(QLineEdit::Normal);
}


void ChPassWindow::on_showSecondNewPassButton_released()
{
    ui->secondNewPassLineEdit->setEchoMode(QLineEdit::Password);
    ui->secondNewPassLineEdit->setFocus();
}
