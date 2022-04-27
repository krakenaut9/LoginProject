#include "questioncheckwindow.h"
#include "ui_questioncheckwindow.h"

QuestionCheckWindow::QuestionCheckWindow(QString userName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionCheckWindow),
    m_userName(userName)
{
    ui->setupUi(this);
}

QuestionCheckWindow::~QuestionCheckWindow()
{
    delete ui;
}
