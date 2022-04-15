#ifndef ACCOUNTEDITORWINDOW_H
#define ACCOUNTEDITORWINDOW_H

#include <QDialog>

namespace Ui {
class AccountEditorWindow;
}

class AccountEditorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AccountEditorWindow(QWidget *parent = nullptr);
    ~AccountEditorWindow();

private:
    Ui::AccountEditorWindow *ui;
};

#endif // ACCOUNTEDITORWINDOW_H
