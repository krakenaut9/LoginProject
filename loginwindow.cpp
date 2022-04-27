#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <manageusers.h>
LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow),
    m_firstLogin(false)
{
    QFile jsonFile(USERS_FILE);
    ui->setupUi(this);
    this->setFixedSize(550, 400);

    if(!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly))
    {
        PLOGE << "Login window : Failed to open json file";
        qDebug() << "Failed to open json file";
        return;
    }
    QJsonParseError JsonParseError;
    m_jsonDocument = QJsonDocument::fromJson(jsonFile.readAll(), &JsonParseError);
    connect(ui->passwordLineEdit, &QLineEdit::returnPressed, this, &LoginWindow::on_loginButton_clicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked()
{

    static bool firstIteration = true;
    static quint8 incorrectPassword;
    QJsonObject RootObject;

    RootObject = m_jsonDocument.object();

    QString userName = ui->usernameLineEdit->text();
    PLOGI << "User " << userName << " tries to login";
    userName = userName.toLower();
    static QString previousUserName = userName;
    if(userName != previousUserName)
    {
        firstIteration = true;
        PLOGW << "Previous name " << previousUserName << " differs from current " << userName;
        qDebug() << "Prev name " << previousUserName << " differs from current " << userName;
    }
    previousUserName = userName;

    QString password = ui->passwordLineEdit->text();

    if(userName.length() == 0)
    {
        PLOGI << "Empty username";
        qDebug()<<"Empty username";
        ui->informLabel->setText("Empty username field");
        ui->usernameLineEdit->setFocus();
        firstIteration = true;
        return;
    }

    QJsonObject::ConstIterator userObjectIterator;
    if((userObjectIterator = RootObject.find(userName)) == RootObject.constEnd())
    {
        PLOGW << "No such user : " << userName;
        qDebug() << "No such user";
        ui->informLabel->setText("No such user");
        ui->usernameLineEdit->setFocus();
        firstIteration = true;
        m_firstLogin = false;
        return;
    }

    if(userObjectIterator.value().toObject()[IS_BLOCKED].toBool())
    {
        PLOGW << "User is blocked : " << userName;
        qDebug()<<"User is blocked";
        ui->informLabel->setText("This user was blocked");
        ui->usernameLineEdit->setFocus();
        return;
    }

    quint64 randomNumber = QRandomGenerator::securelySeeded().generate64();

    if(ManageUsers::encryptPassword(userObjectIterator.value().toObject()[PASSWORD].toString(), randomNumber) != ManageUsers::encryptPassword(password, randomNumber))
    {
        PLOGW << "Incorrect password";
        qDebug() << "Incorrect password";
        ui->informLabel->setText("Incorrect password");
        ui->passwordLineEdit->clear();
        ui->passwordLineEdit->setFocus();
        firstIteration = true;
        if(++incorrectPassword == 3)
        {
            PLOGW << "Incorrect password 3 times.";
            qDebug() << "Incorrect password 3 times.";
            QMessageBox::critical(this, "Exit", "You entered incorrect password three times");
            reject();
        }
        return;
    }

    bool firstLogin = userObjectIterator.value().toObject()[FIRST_LOGIN].toBool();
    if(firstLogin && firstIteration)
    {
        PLOGW << "First login iteration for user : " << userName;
        qDebug() << "First login";
        m_firstLogin = true;
        ui->informLabel->setText("Please enter the password one more time");
        ui->passwordLineEdit->setFocus();
        ui->passwordLineEdit->clear();
        firstIteration = false;
        return;
    }

    if(firstLogin)
    {
        //ManageUsers::changeProperty(userName, FIRST_LOGIN, false);
    }
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
    ui->passwordLineEdit->setFocus();
}

QString LoginWindow::GetUserName()const noexcept
{
    return m_userName;
}
void LoginWindow::SetUserName(const QString& userName)
{
    m_userName = userName;
}

bool LoginWindow::GetFirstLogin()const noexcept
{
    return m_firstLogin;
}
void LoginWindow::SetFirstLogin(const bool isFirstLogin)
{
    m_firstLogin = isFirstLogin;
}

void LoginWindow::on_usernameLineEdit_editingFinished()
{
    ui->passwordLineEdit->setFocus();
}

