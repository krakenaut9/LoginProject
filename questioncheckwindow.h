#ifndef QUESTIONCHECKWINDOW_H
#define QUESTIONCHECKWINDOW_H

#include <QDialog>
#include <pch.h>

#define ADD_ANSWERS "Add answers"
#define ANSWER_THE_QUESTIONS "Answer the questions"

namespace Ui {
class QuestionCheckWindow;
}

class QuestionCheckWindow : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionCheckWindow(QString userName, QString winType, QWidget *parent = nullptr);
    void GetAnswers(QVector<QString>& answers);
    ~QuestionCheckWindow();

private slots:
    void on_nextPushButton_clicked();

    void on_prevPushButton_clicked();

private:
    QVector<QString> m_answers;
    QVector<int> m_questionsToCheck;
    Ui::QuestionCheckWindow *ui;
    QString m_userName;
    QString m_winType;
    quint8 m_questionIndex;
};

#endif // QUESTIONCHECKWINDOW_H
