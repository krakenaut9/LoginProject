#include "accounteditorwindow.h"
#include "ui_accounteditorwindow.h"

AccountEditorWindow::AccountEditorWindow(QString userName,
                                         bool isBlocked,
                                         bool isRestricted,
                                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountEditorWindow),
    m_userName(userName),
    m_password(""),
    m_isBlocked(isBlocked),
    m_isPasswordRestricted(isRestricted)
{
    ui->setupUi(this);
    ui->userNameLineEdit->setText(m_userName);
    ui->blockedCheckBox->setCheckState(isBlocked ? Qt::Checked : Qt::Unchecked);
    ui->restrictedPassCheckBox->setCheckState(isRestricted ? Qt::Checked : Qt::Unchecked);

    ui->changePassCheckBox->setCheckState(Qt::Unchecked);
    ui->passwordLineEdit->setReadOnly(true);
    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Base,Qt::gray);
    palette->setColor(QPalette::Text,Qt::darkGray);
    ui->passwordLineEdit->setPalette(*palette);
}

AccountEditorWindow::~AccountEditorWindow()
{
    delete ui;
}

void AccountEditorWindow::setUserName(const QString& userName)
{
    m_userName = userName;
}
void AccountEditorWindow::setPassword(const QString& password)
{
    m_password = password;
}
void AccountEditorWindow::setBlocked(bool blockedState)
{
    m_isBlocked = blockedState;
}
void AccountEditorWindow::setPasswordRestriction(bool restrictionState)
{
    m_isPasswordRestricted = restrictionState;
}

void AccountEditorWindow::setChangePass(bool changeState)
{
    m_changePass = changeState;
}

QString AccountEditorWindow::getUserName()const noexcept
{
    return m_userName;
}
QString AccountEditorWindow::getPassword()const noexcept
{
    return m_password;
}
bool AccountEditorWindow::getBlockedState()const noexcept
{
    return m_isBlocked;
}
bool AccountEditorWindow::getRestrictedState()const noexcept
{
    return m_isPasswordRestricted;
}

bool AccountEditorWindow::getChangePass()const noexcept
{
    return m_changePass;
}

void AccountEditorWindow::on_okButton_clicked()
{
    m_password = ui->passwordLineEdit->text();
    m_isBlocked = ui->blockedCheckBox->checkState() == Qt::Checked ? true : false;
    m_isPasswordRestricted = ui->restrictedPassCheckBox->checkState() == Qt::Checked ? true : false;
    m_changePass = ui->changePassCheckBox->checkState() == Qt::Checked ? true : false;
}


void AccountEditorWindow::on_showUserPassButton_pressed()
{
    ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
}


void AccountEditorWindow::on_showUserPassButton_released()
{
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->passwordLineEdit->setFocus();
}


void AccountEditorWindow::on_changePassCheckBox_stateChanged(int arg1)
{
    if(!arg1)
    {
        ui->passwordLineEdit->setReadOnly(true);
        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::gray);
        palette->setColor(QPalette::Text,Qt::darkGray);
        ui->passwordLineEdit->setPalette(*palette);
    }
    else
    {
        ui->passwordLineEdit->setReadOnly(false);
        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::white);
        palette->setColor(QPalette::Text,Qt::black);
        ui->passwordLineEdit->setPalette(*palette);
        ui->passwordLineEdit->setFocus();
    }
}

