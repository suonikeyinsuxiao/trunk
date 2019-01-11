#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_login_clicked()
{
    if (tr("123456") == ui->lineEdit_password->text())
    {
        QMessageBox::information(this,"login","登录成功",QMessageBox::Ok,QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::warning(this,"login","登录失败");
    }
}

void Widget::on_pushButton_cancel_clicked()
{
    qApp->quit();
}
