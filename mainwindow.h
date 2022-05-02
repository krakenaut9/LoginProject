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
    MainWindow(const QString& userName, bool activated, QWidget *parent = nullptr);
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
    void activate();

private:
    Ui::MainWindow *ui;
    QString m_userName;
    QTimer* m_timer;
    QFile m_file;
    QAction* m_activateAction;
    bool m_activated;
};
#endif // MAINWINDOW_H
