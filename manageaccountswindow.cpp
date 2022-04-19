#include "manageaccountswindow.h"
#include "ui_manageaccountswindow.h"
#include <accounteditorwindow.h>
#include <pch.h>
#include <manageusers.h>
#include <QMenu>

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
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget->setToolTip("Right click or double click");
    connect(ui->treeWidget,&QTreeWidget::customContextMenuRequested,this,&ManageAccountsWindow::prepareMenu);
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
                EDIT,
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
        if(editor.getChangePass())
        {
            qDebug() << "Change password";
            ManageUsers::changeProperty(userName, PASSWORD, editor.getPassword());
        }
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


void ManageAccountsWindow::on_addButton_clicked()
{
    qDebug() << "Add new clicked";
    AccountEditorWindow editor(ADD_NEW, "", false, false);
    editor.setModal(true);
    auto editorRes = editor.exec();
    if(editorRes == QDialog::Rejected)
    {
        qDebug() << "Editor rejected";
        return;
    }

    QJsonObject userData;
    userData.insert(IS_BLOCKED, editor.getBlockedState());
    userData.insert(PASSWORD, editor.getChangePass() ? editor.getPassword() : "");
    userData.insert(RESTRICTED_PASSWORD, editor.getRestrictedState());
    userData.insert(FIRST_LOGIN, true);
    bool addRes = ManageUsers::addUser(editor.getUserName(), userData);
    if(addRes == false)
    {
        qDebug() << "Failed to add a new user";
        return;
    }
    auto newItem = new QTreeWidgetItem(ui->treeWidget);
    newItem->setText(0, editor.getUserName());
    newItem->setText(1, editor.getBlockedState() ? "true" : "false");
    newItem->setText(2, editor.getRestrictedState() ? "true" : "false");
    ui->treeWidget->addTopLevelItem(newItem);
}

void ManageAccountsWindow::editMenu()
{
    qDebug() << "Edit for " << ui->treeWidget->currentItem()->text(0) << " requested";

    auto item = ui->treeWidget->currentItem();
    on_treeWidget_itemDoubleClicked(item, 0); // second parameter is ignored
}

void ManageAccountsWindow::deleteMenu()
{
    auto item = ui->treeWidget->currentItem();
    auto answer = QMessageBox::question(this, "Delete user", "Are you sure you want to delete this account : " + item->text(0));
    if(answer == QMessageBox::No)
    {
        qDebug() << "Not sure";
        return;
    }
    qDebug() << "Delete for " << item->text(0) << " requested";
    bool deleteRes = ManageUsers::deleteUser(item->text(0));
    if(deleteRes)
    {
        delete item;
    }
}

void ManageAccountsWindow::prepareMenu(const QPoint& pos)
{
    qDebug()<<"Menu for " << ui->treeWidget->currentItem()->text(0) << " requested";

    QMenu menu(this);

    auto actionEdit = menu.addAction("Edit");
    connect(actionEdit, &QAction::triggered, this, &ManageAccountsWindow::editMenu);
    if(ui->treeWidget->currentItem()->text(0) != ADMIN)
    {
        auto actionDelete = menu.addAction("Delete");
        connect(actionDelete, &QAction::triggered, this, &ManageAccountsWindow::deleteMenu);
    }
    menu.exec( ui->treeWidget->mapToGlobal(pos) );
}
