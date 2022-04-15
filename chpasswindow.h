#ifndef CHPASSWINDOW_H
#define CHPASSWINDOW_H

#include <QDialog>
#include <pch.h>
namespace Ui {
class ChPassWindow;
}

class ChPassWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChPassWindow(QString userName, QWidget *parent = nullptr);
    ~ChPassWindow();

public slots:
    void changeButtonClicked();

private slots:
    void on_showOldPassButton_pressed();

    void on_showOldPassButton_released();

    void on_showFirstNewPassButton_pressed();

    void on_showFirstNewPassButton_released();

    void on_showSecondNewPassButton_pressed();

    void on_showSecondNewPassButton_released();

    //void on_oldPassLineEdit_returnPressed();

private:
    Ui::ChPassWindow *ui;
    QString m_userName;
    QJsonDocument m_jsonDocument;
};

#endif // CHPASSWINDOW_H
