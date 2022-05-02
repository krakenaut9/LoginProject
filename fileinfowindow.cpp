#include "fileinfowindow.h"
#include "ui_fileinfowindow.h"

FileInfoWindow::FileInfoWindow(const QString& fileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileInfoWindow),
    m_fileName(fileName)
{
    ui->setupUi(this);
    QFileInfo info(fileName);
    ui->fileNameLabel->setText(info.baseName());

    ui->createdLabelTitle->setText("Created : ");
    ui->createdLabelValue->setText(info.birthTime().toString());

    ui->extentionLabelTitle->setText("Extention : ");
    ui->extentionLabelValue->setText(info.suffix());

    ui->pathLabelTitle->setText("Path : ");
    ui->pathLabelValue->setText(info.absolutePath());

    ui->sizeLabelTitle->setText("Size : ");
    ui->sizeLabelValue->setText(QString::number(info.size()));

    ui->lastModifiedLabelTitle->setText("Last modified : ");
    ui->lastModifiedLabelValue->setText(info.lastModified().toString());

    ui->lastReadLabelTitle->setText("Last read : ");
    ui->lastReadLabelValue->setText(info.lastRead().toString());

    setWindowTitle("File info : " + info.baseName());
}

FileInfoWindow::~FileInfoWindow()
{
    delete ui;
}
