#include "questioncheckwindow.h"
#include "ui_questioncheckwindow.h"
#include <manageusers.h>

QuestionCheckWindow::QuestionCheckWindow(QString userName, QString winType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionCheckWindow),
    m_userName(userName),
    m_winType(winType),
    m_questionIndex(0)
{
    m_answers.resize(QUESTIONS_COUNT);
    ui->setupUi(this);
    setWindowTitle(winType);
    if(winType == ANSWER_THE_QUESTIONS)
    {
        ui->titleLabel->setText("Answer the questions to continue");
    }
    else
    {
        ui->titleLabel->setText("Set the secret answers");
    }
    ui->prevPushButton->setText("Previous");
    ui->prevPushButton->setDisabled(true);

    ui->nextPushButton->setText("Next(" + QString::number(m_questionIndex + 2) + ")");
    ui->questionLabel->setText(s_questionsArray[m_questionIndex]);
    connect(ui->answerLineEdit, &QLineEdit::returnPressed, this, &QuestionCheckWindow::on_nextPushButton_clicked);
    ui->nextPushButton->setDefault(true);
}

QuestionCheckWindow::~QuestionCheckWindow()
{
    delete ui;
}

void QuestionCheckWindow::on_nextPushButton_clicked()
{
    qDebug() << "Next clicked";
    if(ui->answerLineEdit->text().isEmpty())
    {
        qDebug() << "Empty " << m_questionIndex + 1 << " answer";
        return;
    }
    if(m_questionIndex == QUESTIONS_COUNT - 2)
    {
        qDebug() << "Last question";
        ui->nextPushButton->setText("Confirm");
    }
    else
    {
        ui->nextPushButton->setText("Next(" + QString::number(m_questionIndex + 3) + ")");
    }
    m_answers[m_questionIndex] = ui->answerLineEdit->text();
    if(m_questionIndex == QUESTIONS_COUNT - 1)
    {
        qDebug() << "Confirm";
        ManageUsers::addAnswers(m_userName, m_answers);
        accept();
        return;
    }
    if(m_questionIndex == 0)
    {
        ui->prevPushButton->setDisabled(false);
    }


    ui->prevPushButton->setText("Previous(" + QString::number(++m_questionIndex) + ')');
    ui->questionLabel->setText(s_questionsArray[m_questionIndex]);
    if(m_answers[m_questionIndex].isEmpty())
    {
        ui->answerLineEdit->clear();
    }
    else
    {
        ui->answerLineEdit->setText(m_answers[m_questionIndex]);
    }
    ui->answerLineEdit->setFocus();
}


void QuestionCheckWindow::on_prevPushButton_clicked()
{
    qDebug() << "Prev clicked";
    if(m_questionIndex == 1)
    {
        qDebug() << "return to first";
        ui->prevPushButton->setDisabled(true);
    }
    ui->answerLineEdit->setText(m_answers[--m_questionIndex]);
    ui->questionLabel->setText(s_questionsArray[m_questionIndex]);
    ui->prevPushButton->setText("Previous(" + QString::number(m_questionIndex) + ')');
    ui->nextPushButton->setText("Next(" + QString::number(m_questionIndex + 2) + ")");
}

void QuestionCheckWindow::GetAnswers(QVector<QString>& answers)
{
    answers.swap(m_answers);
}
