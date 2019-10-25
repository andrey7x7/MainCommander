#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dirpanel.h"
#include "notepad.h"
#include "QFileSystemWatcher"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void show_bar_l(QString txt);

    void show_bar_r(QString txt);

    void change_dir_l(QString txt);

    void change_dir_r(QString txt);


private slots:
    void on_Button_copy_clicked();

    void on_Button_remove_clicked();

    void on_Button_mkdir_clicked();

    void on_actionOpen_terminal_triggered();

    void on_actionExit_triggered();

    void on_Button_move_clicked();

    void open_file(QString file);
    void changed_file(QString txt);
private:
    Ui::MainWindow *ui;
    DirPanel* panel_l;
    DirPanel* panel_r;
    Notepad* note;
    QFileSystemWatcher *SysWatch;
};

#endif // MAINWINDOW_H
