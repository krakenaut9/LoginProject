#include "captchawindow.h"
#include "ui_captchawindow.h"

CaptchaWindow::CaptchaWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaptchaWindow),
    m_mistakes(0)
{
    ui->setupUi(this);
    m_captcha.randomize();
    m_captcha.setDifficulty(DIFFICULTY);
    m_captcha.generateText(6, true);
    ui->captchaLabel->setPixmap(QPixmap::fromImage(m_captcha.captchaImage()));
    connect(ui->okButton, &QPushButton::clicked, this, &CaptchaWindow::okClicked);
    connect(ui->changeTextButton, &QPushButton::clicked, this, &CaptchaWindow::changeText);
    ui->okButton->setDefault(true);
    setWindowTitle("Captcha test");
}

CaptchaWindow::~CaptchaWindow()
{
    delete ui;
}

void CaptchaWindow::okClicked()
{
    qDebug() << "Ok clicked";
    PLOGI << "User checks captcha";
    if(ui->answerLineEdit->text().isEmpty())
    {
        qDebug() << "Captcha : Empty input";
        PLOGI << "Captcha : Empty input";
        return;
    }
    if(m_captcha.captchaText() == ui->answerLineEdit->text().toUpper())
    {
        accept();
    }
    else
    {
        qDebug() << "Incorrect captcha : " << m_captcha.captchaText() << " != " << ui->answerLineEdit->text().toUpper();
        PLOGI << "Incorrect captcha";
        m_captcha.generateText();
        ui->captchaLabel->setPixmap(QPixmap::fromImage(m_captcha.captchaImage()));
        ui->answerLineEdit->clear();
        ui->answerLineEdit->setFocus();
        if(++m_mistakes == MISTAKES_COUNT)
        {
            qDebug() << MISTAKES_COUNT << " incorrect captcha input";
            PLOGW <<  MISTAKES_COUNT << " incorrect captcha input";
            QMessageBox::critical(this, "Failure", "You entered the wrong captcha text " + QString::number(MISTAKES_COUNT) + " times");
            reject();
        }
    }
}

void CaptchaWindow::changeText()
{
    qDebug() << "User changes text";
    PLOGI << "User changes text";
    m_captcha.generateText(6, true);
    ui->captchaLabel->setPixmap(QPixmap::fromImage(m_captcha.captchaImage()));
    ui->answerLineEdit->clear();
    ui->answerLineEdit->setFocus();
}
