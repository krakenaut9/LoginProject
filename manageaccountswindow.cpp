#include "manageaccountswindow.h"
#include "ui_manageaccountswindow.h"
#include <accounteditorwindow.h>
#include <pch.h>
#include <manageusers.h>

ManageAccountsWindow::ManageAccountsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageAccountsWindow)
{
    QFile jsonFile(USERS_FILE);
    ui->setupUi(this);

    if(!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly))
    {
        qDebug() << "Failed to open json file";
        return;
    }
    QJsonParseError JsonParseError;
    auto jsonDocument = QJsonDocument::fromJson(jsonFile.readAll(), &JsonParseError);
    auto rootObject = jsonDocument.object();
    for(auto it = rootObject.constBegin(); it != rootObject.constEnd(); ++it)
    {
        auto item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, it.key());
        item->setText(1, it.value().toObject()[IS_BLOCKED].toBool() ? "true" : "false");
        item->setText(2, it.value().toObject()[RESTRICTED_PASSWORD].toBool() ? "true" : "false");
        ui->treeWidget->addTopLevelItem(item);
    }
}

ManageAccountsWindow::~ManageAccountsWindow()
{
    delete ui;
}

void ManageAccountsWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString userName = item->text(0);
    bool isBlocked = item->text(1) == "true" ? true : false;
    bool isRestricted = item->text(2) == "true" ? true : false;
    qDebug() << "Double clicked : " << userName;
    AccountEditorWindow editor(
                userName,
                isBlocked,
                isRestricted
                );
    editor.setModal(true);
    auto execRes = editor.exec();
    if(execRes == QDialog::Rejected)
    {
        qDebug() << "Editor rejected";
        return;
    }
    else if(execRes == QDialog::Accepted)
    {
        qDebug() << "Editor accepted";
        editor.getBlockedState();
        ManageUsers::changeProperty(userName, PASSWORD, editor.getPassword());
        if(isBlocked != editor.getBlockedState())
        {
            qDebug() << "Change blocked state";
            ManageUsers::changeProperty(userName, IS_BLOCKED, editor.getBlockedState());
            item->setText(1, editor.getBlockedState() ? "true" : "false");
        }
        if(isRestricted != editor.getRestrictedState())
        {
            qDebug() << "Change restricted state";
            ManageUsers::changeProperty(userName, RESTRICTED_PASSWORD, editor.getRestrictedState());
            item->setText(2, editor.getRestrictedState() ? "true" : "false");
        }
        return;
    }
    qDebug() << "I don't know how did you got here lol\n"
                "Just a kek comment 15.04.22 23:13";
}

