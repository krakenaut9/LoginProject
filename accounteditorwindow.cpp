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

void AccountEditorWindow::on_okButton_clicked()
{
    m_password = ui->passwordLineEdit->text();
    m_isBlocked = ui->blockedCheckBox->checkState() == Qt::Checked ? true : false;
    m_isPasswordRestricted = ui->restrictedPassCheckBox->checkState() == Qt::Checked ? true : false;
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

