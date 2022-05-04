#ifndef CAPTCHAWINDOW_H
#define CAPTCHAWINDOW_H

#include <QDialog>
#include <captcha.h>
#include <QPixmap>
#include <pch.h>

#define DIFFICULTY 3
#define MISTAKES_COUNT 3

namespace Ui {
class CaptchaWindow;
}

class CaptchaWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CaptchaWindow(QWidget *parent = nullptr);
    ~CaptchaWindow();
public slots:
    void okClicked();
    void changeText();
private:
    Ui::CaptchaWindow *ui;
    Captcha m_captcha;
    quint8 m_mistakes;
};

#endif // CAPTCHAWINDOW_H
