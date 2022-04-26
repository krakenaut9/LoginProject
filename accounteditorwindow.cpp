#include "accounteditorwindow.h"
#include "ui_accounteditorwindow.h"

AccountEditorWindow::AccountEditorWindow(QString winType,
                                         QString userName,
                                         QString accessLevel,
                                         bool isBlocked,
                                         bool isRestricted,
                                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountEditorWindow),
    m_userName(userName),
    m_password(""),
    m_winType(winType),
    m_accessLevel(accessLevel),
    m_isBlocked(isBlocked),
    m_isPasswordRestricted(isRestricted)
{
    ui->setupUi(this);
    ui->userNameLineEdit->setText(m_userName);
    ui->blockedCheckBox->setCheckState(isBlocked ? Qt::Checked : Qt::Unchecked);
    ui->restrictedPassCheckBox->setCheckState(isRestricted ? Qt::Checked : Qt::Unchecked);
    if(winType == EDIT)
    {
        ui->changePassCheckBox->setCheckState(Qt::Unchecked);
        ui->passwordLineEdit->setReadOnly(true);
        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::gray);
        palette->setColor(QPalette::Text,Qt::darkGray);
        ui->passwordLineEdit->setPalette(*palette);
    }
    else if(winType == ADD_NEW)
    {
        ui->userNameLineEdit->setReadOnly(false);
        ui->changePassCheckBox->setCheckState(Qt::Checked);
        ui->userNameLineEdit->setFocus();
    }

    if(userName == ADMIN)
    {
        ui->blockedCheckBox->setDisabled(true);
        ui->accessLevelComboBox->setDisabled(true);
    }

    if(winType == EDIT)
    {
        ui->changePassCheckBox->setToolTip("Also change a password");
    }
    else if(winType == ADD_NEW)
    {
        ui->changePassCheckBox->setToolTip("Empty/not empty password");
    }

    ui->accessLevelComboBox->addItem(ACCESS_LEVEL_DEFAULT);
    ui->accessLevelComboBox->addItem(ACCESS_LEVEL_ADMIN);
    ui->accessLevelComboBox->setCurrentText(m_accessLevel);
    ui->restrictedPassCheckBox->setToolTip("User can change his password only to one that contains numbers and symbols +-*/=%()^:");
    ui->restrictedPassLabel->setToolTip("User can change his password only to one that contains numbers and symbols +-*/=%()^:");
    setWindowTitle(winType);
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

void AccountEditorWindow::setAccessLevel(const QString level)
{
    m_accessLevel = level;
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

QString AccountEditorWindow::getAccessLevel()const noexcept
{
    return m_accessLevel;
}

void AccountEditorWindow::on_okButton_clicked()
{
    m_userName = ui->userNameLineEdit->text();
    m_password = ui->passwordLineEdit->text();
    m_isBlocked = ui->blockedCheckBox->checkState() == Qt::Checked ? true : false;
    m_isPasswordRestricted = ui->restrictedPassCheckBox->checkState() == Qt::Checked ? true : false;
    m_changePass = ui->changePassCheckBox->checkState() == Qt::Checked ? true : false;
    m_accessLevel = ui->accessLevelComboBox->currentText();
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

