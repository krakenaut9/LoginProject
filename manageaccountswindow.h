#ifndef MANAGEACCOUNTSWINDOW_H
#define MANAGEACCOUNTSWINDOW_H

#include <QDialog>
#include <QTreeWidgetItem>
namespace Ui {
class ManageAccountsWindow;
}

class ManageAccountsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ManageAccountsWindow(QWidget *parent = nullptr);
    ~ManageAccountsWindow();

private slots:
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_addButton_clicked();
    void prepareMenu(const QPoint& pos);
    void editMenu();
    void deleteMenu();

private:
    Ui::ManageAccountsWindow *ui;
};

#endif // MANAGEACCOUNTSWINDOW_H
