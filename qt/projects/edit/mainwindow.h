#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>
#include <QLabel>
#include <QHBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    bool _save_file(const QString& filename);
    void initStatusBar();
    void isSaveOrNot();
    void open_file();
    void new_file();
    void save_file();
    void saveas_file();
    void close_file();

private slots:
    void on_action_new_triggered();

    void on_action_save_triggered();

    void action_change_window_title();
    void action_cursor_changed();

    void on_action_saveas_triggered();

    void on_action_open_triggered();

    void on_action_close_triggered();

    void on_action_undo_triggered();

    void on_action_cut_triggered();

    void on_action_copy_triggered();

    void on_action_paste_triggered();

    void on_action_about_triggered();

private:
    Ui::MainWindow *ui;
    QString curFile;
    bool isSaved;
    QLabel* statusLabel;
    QLabel* statusLabel2;
    QHBoxLayout *layout;
};


#endif // MAINWINDOW_H
