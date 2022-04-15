#include "accounteditorwindow.h"
#include "ui_accounteditorwindow.h"

AccountEditorWindow::AccountEditorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountEditorWindow)
{
    ui->setupUi(this);
}

AccountEditorWindow::~AccountEditorWindow()
{
    delete ui;
}
