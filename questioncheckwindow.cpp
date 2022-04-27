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
    if(winType == ADD_ANSWERS)
        m_answers.resize(QUESTIONS_COUNT);
    else
    {
        m_answers.resize(QUESTIONS_TO_CHECK);
        m_questionsToCheck.resize(QUESTIONS_TO_CHECK);
        for(size_t i = 0; i < QUESTIONS_TO_CHECK; ++i)
        {
            while(true)
            {
                int rand = QRandomGenerator::securelySeeded().bounded(0, QUESTIONS_COUNT);
                if(m_questionsToCheck.contains(rand))
                {
                    continue;
                }
                m_questionsToCheck[i] = rand;
                break;
            }
            qDebug() << "Add random question : " << m_questionsToCheck[i];
        }
    }
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
    if(m_winType == ADD_ANSWERS)
    {
        ui->questionLabel->setText(s_questionsArray[m_questionIndex]);
    }
    else
    {
        qDebug() << "s_questionsArray[m_questionsToCheck[0]]" << s_questionsArray[m_questionsToCheck[0]];
        ui->questionLabel->setText(s_questionsArray[m_questionsToCheck[0]]);
    }
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
    if(m_questionIndex == (m_winType == ADD_ANSWERS ? QUESTIONS_COUNT : QUESTIONS_TO_CHECK) - 2)
    {
        qDebug() << "Last question";
        ui->nextPushButton->setText("Confirm");
    }
    else
    {
        ui->nextPushButton->setText("Next(" + QString::number(m_questionIndex + 3) + ")");
    }

    ui->prevPushButton->setText("Previous(" + QString::number(m_questionIndex + 1) + ')');
    m_answers[m_questionIndex] = ui->answerLineEdit->text();


    if(m_winType == ADD_ANSWERS)
    {

        if(m_questionIndex == QUESTIONS_COUNT - 1)
        {
            qDebug() << "Confirm";
            ManageUsers::addAnswers(m_userName, m_answers);

            accept();
            return;
        }

        ui->questionLabel->setText(s_questionsArray[m_questionIndex+1]);
    }
    else
    {
        if(m_questionIndex == QUESTIONS_TO_CHECK - 1)
        {
            qDebug() << "Confirm check";
            if(ManageUsers::isAnswersCorrect(m_userName, m_answers, m_questionsToCheck))
            {
                qDebug() << "Answers are correct";
                accept();
                return;
            }
            else
            {
                qDebug() << "Answers are not correct";
                reject();
                return;
            }
        }

        ui->questionLabel->setText(s_questionsArray[m_questionsToCheck[m_questionIndex+1]]);
    }
    if(m_questionIndex == 0)
    {
        ui->prevPushButton->setDisabled(false);
    }


    ++m_questionIndex;
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
    if(m_winType == ADD_ANSWERS)
        ui->questionLabel->setText(s_questionsArray[m_questionIndex]);
    else
        ui->questionLabel->setText(s_questionsArray[m_questionsToCheck[m_questionIndex]]);
    ui->prevPushButton->setText("Previous(" + QString::number(m_questionIndex) + ')');
    ui->nextPushButton->setText("Next(" + QString::number(m_questionIndex + 2) + ")");
}

void QuestionCheckWindow::GetAnswers(QVector<QString>& answers)
{
    answers.swap(m_answers);
}
