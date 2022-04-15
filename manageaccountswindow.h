#ifndef MANAGEACCOUNTSWINDOW_H
#define MANAGEACCOUNTSWINDOW_H

#include <QDialog>

namespace Ui {
class ManageAccountsWindow;
}

class ManageAccountsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ManageAccountsWindow(QWidget *parent = nullptr);
    ~ManageAccountsWindow();

private:
    Ui::ManageAccountsWindow *ui;
};

#endif // MANAGEACCOUNTSWINDOW_H
