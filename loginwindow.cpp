#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <manageusers.h>
LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)

{
    QFile jsonFile(USERS_FILE);
    ui->setupUi(this);
    this->setFixedSize(550, 400);

    if(!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly))
    {
        qDebug() << "Failed to open json file";
        return;
    }
    QJsonParseError JsonParseError;
    m_jsonDocument = QJsonDocument::fromJson(jsonFile.readAll(), &JsonParseError);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked()
{
    static bool firstIteration = true;

    QJsonObject RootObject;

    RootObject = m_jsonDocument.object();

    QString userName = ui->usernameLineEdit->text();
    static QString previousUserName;
    if(userName != previousUserName)
    {
        firstIteration = true;
        qDebug() << "Prev name " << previousUserName << " differs from current " << userName;
    }
    previousUserName = userName;

    QString password = ui->passwordLineEdit->text();

    if(userName.length() == 0)
    {
        qDebug()<<"Empty username";
        ui->informLabel->setText("Empty username field");
        ui->usernameLineEdit->setFocus();
        firstIteration = true;
        return;
    }

    QJsonObject::ConstIterator userObjectIterator;
    if((userObjectIterator = RootObject.find(userName)) == RootObject.constEnd())
    {
        qDebug() << "No such user";
        ui->informLabel->setText("No such user");
        ui->usernameLineEdit->setFocus();
        firstIteration = true;
        return;
    }


    if(userObjectIterator.value().toObject()[PASSWORD] != password)
    {
        qDebug() << "Incorrect password";
        ui->informLabel->setText("Incorrect password");
        ui->passwordLineEdit->clear();
        ui->passwordLineEdit->setFocus();
        firstIteration = true;
        return;
    }


    if(userObjectIterator.value().toObject()[FIRST_LOGIN] == true && firstIteration == true)
    {
        qDebug() << "First login";
        ui->informLabel->setText("Please enter the password one more time");
        ui->passwordLineEdit->setFocus();
        ui->passwordLineEdit->clear();
        firstIteration = false;
        return;
    }

    ManageUsers::changeProperty(userName, FIRST_LOGIN, false);

    accept();
    SetUserName(userName);
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
