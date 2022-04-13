#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& userName, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void changeMyPassword();
    void showAccountInformation();
    void manageAccounts();
private:
    Ui::MainWindow *ui;
    QString m_userName;
};
#endif // MAINWINDOW_H
