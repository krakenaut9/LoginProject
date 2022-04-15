#include "manageaccountswindow.h"
#include "ui_manageaccountswindow.h"
#include <pch.h>

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
    qDebug() << "Double clicked : " << item->text(0);

}

