#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent, QString on_file) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    ui->setupUi(this);
    this->on_file=on_file;

    //слежение за файлом
    SysWatch = new QFileSystemWatcher(this);
    SysWatch->addPath(on_file);
    connect(SysWatch,SIGNAL(fileChanged(QString)),this,SLOT(changed_file(QString)));

    open_file();
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::changed_file(QString txt)
{
    ui->statusbar->showMessage(txt);
}


void Notepad::open_file()
{
    QFile file;

    QString stroka;

    file.setFileName(on_file);

    file.open(QIODevice::ReadOnly|QIODevice::Text); //открытие файла

    QTextStream read(&file);

    ui->textEdit->setText(read.readAll());

    file.close();
}

void Notepad::on_actionClose_triggered()
{
    close();
}

void Notepad::on_actionSave_triggered()
{
    QFile file;

    QString stroka;

    file.setFileName(on_file);

    file.open(QIODevice::WriteOnly|QIODevice::Text); //открытие файла

    QTextStream save(&file);

    stroka = ui->textEdit->toPlainText();

    save<<stroka;

    file.close();
}
