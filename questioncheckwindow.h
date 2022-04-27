#ifndef QUESTIONCHECKWINDOW_H
#define QUESTIONCHECKWINDOW_H

#include <QDialog>
#include <pch.h>

namespace Ui {
class QuestionCheckWindow;
}

class QuestionCheckWindow : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionCheckWindow(QString userName, QWidget *parent = nullptr);
    ~QuestionCheckWindow();

private:
    Ui::QuestionCheckWindow *ui;
    QString m_userName;
};

#endif // QUESTIONCHECKWINDOW_H
