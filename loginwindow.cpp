#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    m_jsonFile(USERS_FILE),
    ui(new Ui::LoginWindow)

{
    ui->setupUi(this);
    this->setFixedSize(550, 400);

    if(!m_jsonFile.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly))
    {
        qDebug() << "Failed to open json file";
        return;
    }
    QJsonParseError JsonParseError;
    m_jsonDocument = QJsonDocument::fromJson(m_jsonFile.readAll(), &JsonParseError);

}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked()
{
    QJsonObject RootObject;

    RootObject = m_jsonDocument.object();

    QString userName = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if(userName.length() == 0)
    {
        qDebug()<<"Empty username";
        ui->informLabel->setText("Empty username field");
        ui->usernameLineEdit->setFocus();
        return;
    }

    QJsonObject::ConstIterator userObjectIterator;
    if((userObjectIterator = RootObject.find(userName)) == RootObject.constEnd())
    {
        qDebug() << "No such user";
        ui->informLabel->setText("No such user");
        ui->usernameLineEdit->setFocus();
        return;
    }

    if(userObjectIterator.value().toObject()["Password"] != password)
    {
        qDebug() << "Incorrect password";
        ui->informLabel->setText("Incorrect password");
        ui->usernameLineEdit->setFocus();
        return;
    }
    //QJsonObject userObject = RootObject.find(userName).value().toObject();
    //userObject["Password"];
    accept();
    SetUserName(userName);
    m_jsonFile.close();
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
