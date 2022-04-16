#ifndef ACCOUNTEDITORWINDOW_H
#define ACCOUNTEDITORWINDOW_H

#include <QDialog>
#include <pch.h>

namespace Ui {
class AccountEditorWindow;
}

class AccountEditorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AccountEditorWindow(QString userName,
                                 bool isBlocked,
                                 bool isRestricted,
                                 QWidget *parent = nullptr);
    ~AccountEditorWindow();

    void setUserName(const QString& userName);
    void setPassword(const QString& password);
    void setBlocked(bool blockedState);
    void setPasswordRestriction(bool restrictionState);
    void setChangePass(bool changeState);

    QString getUserName()const noexcept;
    QString getPassword()const noexcept;
    bool getBlockedState()const noexcept;
    bool getRestrictedState()const noexcept;
    bool getChangePass()const noexcept;
private slots:
    void on_okButton_clicked();

    void on_showUserPassButton_pressed();

    void on_showUserPassButton_released();

    void on_changePassCheckBox_stateChanged(int arg1);

private:
    Ui::AccountEditorWindow *ui;
    QString m_userName;
    QString m_password;
    bool m_isBlocked;
    bool m_isPasswordRestricted;
    bool m_changePass;
};

#endif // ACCOUNTEDITORWINDOW_H
