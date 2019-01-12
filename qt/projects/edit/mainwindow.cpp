#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextCursor>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Easynote"));
    ui->menuBar->setWindowTitle(tr("Easynote"));
    ui->textEdit->setVisible(false);

    initStatusBar();
    curFile = "default.txt";

    connect(ui->textEdit,SIGNAL(textChanged()),this,SLOT(action_change_window_title()));
    connect(ui->textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(action_cursor_changed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::action_change_window_title()
{
    setWindowTitle(tr("%1 %2").arg(curFile).arg(tr("*")));

}

void MainWindow::action_cursor_changed()
{
    //获取行号和列号
    QTextCursor cursor = ui->textEdit->textCursor();
    statusLabel2->setVisible(true);
    statusLabel2->setText(tr("行:%1,列:%2").arg(cursor.blockNumber()).arg(cursor.columnNumber()));
}

void MainWindow::initStatusBar()
{
    QStatusBar* bar  = ui->statusBar;

    //bar->showMessage("Ready",2000);//状态栏简单的信息显示

    //创建两个label，一个显示每次操作的状态信息，一个显示行列信息
    statusLabel = new QLabel(bar);
    statusLabel->setMinimumSize(300,20);//设置标签尺寸
    statusLabel->setFrameShape(QFrame::WinPanel);//设置标签形状
    statusLabel->setFrameShadow(QFrame::Sunken);//设置标签阴影
    statusLabel->setText(tr("欢迎使用Easynote"));
    //qDebug()<< statusLabel->geometry() << endl;

    statusLabel2 = new QLabel(bar);
    statusLabel2->setMinimumSize(300,20);//设置标签尺寸
    statusLabel2->setFrameShape(QFrame::WinPanel);//设置标签形状
    statusLabel2->setFrameShadow(QFrame::Sunken);//设置标签阴影
    statusLabel2->setVisible(false);

    bar->addWidget(statusLabel);
    bar->addWidget(statusLabel2);

}

void MainWindow::isSaveOrNot()
{
    qDebug() << "is save or not" << endl;
    if (ui->textEdit->document()->isModified())//修改过
    {
        //FileInfo.setFile("./test.txt");
        //qDebug() << FileInfo.absolutePath() << endl;
        //qDebug() << FileInfo.absoluteFilePath() << endl;
        //qDebug() << FileInfo.canonicalFilePath() << endl;
        QMessageBox box;
        box.setWindowTitle(tr("提示"));
        box.setText(tr("是否保存当前文件: %1").arg(curFile));
        box.setIcon(QMessageBox::Information);
        box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        if (box.exec() == QMessageBox::Yes)
        {
            save_file();
        }
    }

}

void MainWindow::new_file()
{
    //判断当前有文件打开,有，则提示是否保存当前文件
    isSaveOrNot();

    qDebug() << "new file" << endl;

    isSaved = false;//设置文件为未保存状态
    curFile = "default.txt";
    setWindowTitle(curFile);

    ui->textEdit->clear();
    ui->textEdit->setVisible(true);

    statusLabel->setText(tr("创建成功"));
}

bool MainWindow::_save_file(const QString &fileName)
{
    qDebug() << "save file" << endl;
    //创建文件
    QFile file(fileName);
    //open
    if (!file.open(QFile::WriteOnly|QFile::Text))
    {
        QMessageBox::warning(this,tr("打开文件"),tr("打开文件 %1 失败").arg(curFile));
        return false;
    }

    //写文件
    //file.write(ui->textEdit->toPlainText().toUtf8().data());

    //写文件
    //创建流对象
    QTextStream out(&file);
    //将textedit的内容保存到文件中
    out << ui->textEdit->toPlainText();

    ui->textEdit->document()->setModified(false);//复位修改标志
    isSaved = true;
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);

    file.close();

    statusLabel->setText(tr("保存成功"));
    return true;
}

void MainWindow::save_file()
{
    if (isSaved)
    {
        _save_file(curFile);
    }
    else
    {
        saveas_file();
    }

}

void MainWindow::saveas_file()
{
    qDebug() << "saveas file" << endl;
    //文件另存为对话框
    curFile = QFileDialog::getSaveFileName(this,tr("文件另存为"),curFile);

    if (!curFile.isEmpty())
        _save_file(curFile);
}

void MainWindow::open_file()
{
    //是否要保存当前文件
    //isSaveOrNot();
    //选择文件对话框
    QString filename = QFileDialog::getOpenFileName(this);
    if (!filename.isEmpty())
    {
        //选择文件后，怎样获取文件的内容
        //将获取的文件内容显示到textedit中

        QFile file(filename);
        if (!file.open(QFile::ReadOnly|QFile::Text))
        {
            QMessageBox::warning(this,tr("打开文件"),tr("打开文件%1失败").arg(filename));
            return;
        }

        QTextStream in(&file);
        ui->textEdit->setText(in.readAll());

        curFile = QFileInfo(filename).canonicalFilePath();
        setWindowTitle(curFile);
        file.close();
    }
    ui->textEdit->setVisible(true);
    statusLabel->setText(tr("打开成功"));
}

void MainWindow::on_action_new_triggered()
{
    new_file();
}

void MainWindow::on_action_save_triggered()
{
    //isSaveOrNot();
    if (ui->textEdit->document()->isModified())//修改过
        save_file();
}

void MainWindow::on_action_saveas_triggered()
{
    saveas_file();
}

void MainWindow::on_action_open_triggered()
{
    open_file();
}

void MainWindow::on_action_close_triggered()
{
    save_file();
    ui->textEdit->setVisible(false);
    statusLabel->setText(tr("欢迎使用"));
    statusLabel2->setVisible(false);
}

void MainWindow::on_action_undo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_action_cut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_action_copy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_action_paste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_action_about_triggered()
{

}
