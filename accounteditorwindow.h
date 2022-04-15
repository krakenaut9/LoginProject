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

    QString getUserName()const noexcept;
    QString getPassword()const noexcept;
    bool getBlockedState()const noexcept;
    bool getRestrictedState()const noexcept;
private slots:
    void on_okButton_clicked();

    void on_showUserPassButton_pressed();

    void on_showUserPassButton_released();

private:
    Ui::AccountEditorWindow *ui;
    QString m_userName;
    QString m_password;
    bool m_isBlocked;
    bool m_isPasswordRestricted;
};

#endif // ACCOUNTEDITORWINDOW_H
