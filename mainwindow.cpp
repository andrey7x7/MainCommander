#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QListWidgetItem"
#include "QDebug"
#include "QDir"
#include "QFile"
#include "QInputDialog"
#include "QMessageBox"
#include "QTemporaryFile"

bool select_left;
QString dir_l,dir_r;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    select_left=true;
    dir_l="/home";
    dir_r="/home";

    panel_l = new DirPanel(this);
    panel_r = new DirPanel(this);
    panel_l->setDirLoc(dir_l);
    panel_l->setDirDst(dir_r);
    panel_r->setDirLoc(dir_r);
    panel_r->setDirDst(dir_l);

    connect(panel_l,SIGNAL(select_dir_file(QString)),this,SLOT(show_bar_l(QString)));
    connect(panel_r,SIGNAL(select_dir_file(QString)),this,SLOT(show_bar_r(QString)));

    connect(panel_l,SIGNAL(change_dir(QString)),this,SLOT(change_dir_l(QString)));
    connect(panel_r,SIGNAL(change_dir(QString)),this,SLOT(change_dir_r(QString)));

    connect(panel_l,SIGNAL(update_dir()),panel_r,SLOT(file_in_dir()));
    connect(panel_r,SIGNAL(update_dir()),panel_l,SLOT(file_in_dir()));

    connect(panel_l,SIGNAL(open_file(QString)),this,SLOT(open_file(QString)));
    connect(panel_r,SIGNAL(open_file(QString)),this,SLOT(open_file(QString)));

    ui->gridLayout->addWidget(panel_l,0,0);
    ui->gridLayout->addWidget(panel_r,0,1);

    SysWatch = new QFileSystemWatcher(this);
    connect(SysWatch,SIGNAL(directoryChanged(QString)),this,SLOT(changed_file(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::changed_file(QString txt)
{
    ui->statusBar->showMessage(txt);
}

void MainWindow::on_actionOpen_terminal_triggered()
{
    QString comand=QString("mate-terminal --working-directory=%1").arg(dir_l);
    system(qPrintable(comand));
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_Button_copy_clicked()
{
//    if(dir_l==dir_r)QMessageBox::warning(this, "Внимание","Некорректная операция");
//    else
//    {
        if(select_left==true)panel_l->on_copy();
        if(select_left==false)panel_r->on_copy();
//    }
}

void MainWindow::on_Button_remove_clicked()
{
    if(select_left==true)panel_l->on_remove();
    if(select_left==false)panel_r->on_remove();
}

void MainWindow::on_Button_mkdir_clicked()
{
    if(select_left==true)panel_l->on_mkdir();
    if(select_left==false)panel_r->on_mkdir();
}

void MainWindow::on_Button_move_clicked()
{
    if(dir_l==dir_r)QMessageBox::warning(this, "Внимание","Некорректная операция");
    else
    {
        if(select_left==true)panel_l->on_move();
        if(select_left==false)panel_r->on_move();
    }
}

void MainWindow::show_bar_l(QString txt)
{
    select_left=true;
    ui->statusBar->showMessage(dir_l+"/"+txt);
}

void MainWindow::show_bar_r(QString txt)
{
    select_left=false;
    ui->statusBar->showMessage(dir_r+"/"+txt);
}

void MainWindow::change_dir_l(QString txt)
{
    dir_l=txt;panel_r->setDirDst(txt);
    SysWatch->addPath(txt);
}

void MainWindow::change_dir_r(QString txt)
{
    dir_r=txt;panel_l->setDirDst(txt);
    SysWatch->addPath(txt);
}

void MainWindow::open_file(QString file)
{
    if(select_left==true)
    {
        note = new Notepad(this,dir_l+"/"+file);
        note->show();
    }
    if(select_left==false)
    {
        note = new Notepad(this,dir_r+"/"+file);
        note->show();
    }
}

