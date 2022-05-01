#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pch.h>

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
    void manageAccounts();
    void aboutAuthor();
    void usedTechnologies();
    void reAuthTimer();
    void openFile();
    void newFile();
    void saveFile();
    void saveAsFile();
    void parameters();
    void printFile();

private:
    Ui::MainWindow *ui;
    QString m_userName;
    QTimer* m_timer;
    QFile m_file;
};
#endif // MAINWINDOW_H
