#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int num1 = ui->lineEdit_1->text().toInt();
    int num2 = ui->lineEdit_2->text().toInt();

    QString oper = ui->lineEdit_oper->text();

    if (oper == QString("+"))
    {
        qDebug() << tr("加法")<<endl;
        ui->lineEdit_result->setText(QString::number(num1+num2));
    }
    else if (oper == QString("-"))
    {
        qDebug() << tr("减法")<<endl;
        ui->lineEdit_result->setText(QString::number(num1-num2));
    }
    else if (oper == QString("*"))
    {
        qDebug() << tr("乘法")<<endl;
        ui->lineEdit_result->setText(QString::number(num1*num2));
    }
    else if (oper == QString("/"))
    {
        qDebug() << tr("除法")<<endl;
        ui->lineEdit_result->setText(QString::number((double)num1/num2));

    }
}
