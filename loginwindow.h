#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

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

private slots:
    void on_loginButton_clicked();

    void on_showButton_pressed();

    void on_showButton_released();

private:
    Ui::LoginWindow *ui;
    QString m_userName;
};

#endif // LOGINWINDOW_H
