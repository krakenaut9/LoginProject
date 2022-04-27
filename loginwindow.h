#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <pch.h>
#include <QDialog>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    QString GetUserName()const noexcept;
    void SetUserName(const QString& userName);

    bool GetFirstLogin()const noexcept;
    void SetFirstLogin(const bool isFirstLogin);

private slots:
    void on_loginButton_clicked();

    void on_showButton_pressed();

    void on_showButton_released();

    void on_usernameLineEdit_editingFinished();

private:
    Ui::LoginWindow *ui;
    QString m_userName;
    QJsonDocument m_jsonDocument;
    bool m_firstLogin;
};

#endif // LOGINWINDOW_H
