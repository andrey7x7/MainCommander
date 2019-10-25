#include "dirpanel.h"
#include "ui_dirpanel.h"
#include "QListWidget"
#include "QFile"
#include "QDir"
#include "QInputDialog"
#include "QDebug"
#include "QMessageBox"
DirPanel::DirPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DirPanel)
{
    ui->setupUi(this);
}

DirPanel::~DirPanel()
{
    delete ui;
}

void DirPanel::file_in_dir()
{
    ui->label->setText(dir_loc);
    ui->listWidget->clear();
    QDir dir(dir_loc);
    QFileInfoList list = dir.entryInfoList();     //получаем список файлов директории

    for(int i = 0;i<list.size();i++)
    {
    QListWidgetItem *item1;
    QFileInfo fileInfo=list.at(i);
    if(fileInfo.isDir())item1 = new QListWidgetItem(QIcon(":/res/img/fold_1.png"),fileInfo.fileName(),NULL,1);
    if(fileInfo.isFile())item1 = new QListWidgetItem(QIcon(":/res/img/file_1.png"),fileInfo.fileName(),NULL,2);
    if(fileInfo.fileName()!=".")ui->listWidget->addItem(item1);//каталог с '.' не выводим
    }
}

QString DirPanel::up_dir(QString adres)//переход на уровень выше
{
    QStringList mas = adres.split('/');
    adres = adres.left((adres.indexOf(mas[mas.size()-1]))-1);//удаляем до / справа
    if(adres=="" || adres=="//")adres="/"; //если корень диска
    return adres;
}

void DirPanel::on_listWidget_doubleClicked(const QModelIndex &index)
{
    QString ind = ui->listWidget->currentItem()->text();
    if(ui->listWidget->currentItem()->type()==1)//если item каталог
    {
        if(ind=="..")//признак перехода на уровень выше
        {
            dir_loc=up_dir(dir_loc);
            file_in_dir();
        }
        else
        {
            dir_loc+="/";
            dir_loc+=ind;
            file_in_dir();
        }
        emit change_dir(dir_loc);
    }
    else
        emit open_file(ind);

}

void DirPanel::on_listWidget_itemSelectionChanged()
{
    name = ui->listWidget->currentItem()->text();
    emit  select_dir_file(name);
}



void DirPanel::on_mkdir()//создание каталога
{
    QInputDialog *ddd = new QInputDialog (this);
    ddd->setWindowFlags(Qt::WindowStaysOnTopHint);
    QString text = ddd->getText(0," ","Введите имя каталога: ",QLineEdit::Normal);
    if(text!=NULL)
    {
        if(QDir(QString("%1/%2").arg(dir_loc).arg(text)).exists())
            QMessageBox::warning(this, "Внимание","Каталог уже существует");
        else
            QDir().mkdir(QString("%1/%2").arg(dir_loc).arg(text));

        file_in_dir();
        emit update_dir();
    }
    else QMessageBox::warning(this, "Внимание","Имя каталога некорректно");
}

void DirPanel::on_copy()//копирование
{
    if(name=="..")QMessageBox::warning(this, "Внимание","Эту операцию невозможно выполнить на \"..\"");
    else
    {
        if(QFile(QString("%1/%2").arg(dir_dst).arg(name)).exists())
        {
            if(OK_cancel("Файл с таким именем уже существует. \n Заменить?"))
            {
                 QFile::remove(QString("%1/%2").arg(dir_dst).arg(name));
                 QFile::copy(dir_loc+"/"+name, dir_dst+"/"+name);
            }
        }
        else
            QFile::copy(dir_loc+"/"+name, dir_dst+"/"+name);

        file_in_dir();
        emit update_dir();
    }
}

void DirPanel::on_remove()//удаление
{
    if(name=="..")QMessageBox::warning(this, "Внимание","Эту операцию невозможно выполнить на \"..\"");
    else
    {
        if(name!="")
        {
            if(OK_cancel("Подтвердите удаление ?"))
                if(QFileInfo(QString("%1/%2").arg(dir_loc).arg(name)).isFile())
                {
                    QFile().remove(QString("%1/%2").arg(dir_loc).arg(name));
                }

                if(QFileInfo(QString("%1/%2").arg(dir_loc).arg(name)).isDir())
                {
                    QDir().rmdir(QString("%1/%2").arg(dir_loc).arg(name));
                }
        }

        file_in_dir();
        emit update_dir();
    }
}

void DirPanel::on_move()//перемещение
{
    if(name=="..")QMessageBox::warning(this, "Внимание","Эту операцию невозможно выполнить на \"..\"");
    else
    {
        if(name!="")
        {
            if(QFile(QString("%1/%2").arg(dir_dst).arg(name)).exists())
            {
                if(OK_cancel("Файл с таким именем уже существует. \n Заменить?"))
                {
                     QFile::remove(QString("%1/%2").arg(dir_dst).arg(name));
                     QFile::rename(dir_loc+"/"+name, dir_dst+"/"+name);
                }
            }
            else
            QFile::rename(dir_loc+"/"+name, dir_dst+"/"+name);
        }

        file_in_dir();
        emit update_dir();
    }
}

bool DirPanel::OK_cancel(QString mesaga)
{
    QMessageBox OK;
    OK.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
    OK.setStyleSheet("font: 75 14pt ");
    OK.setText(mesaga);
    OK.setWindowTitle(" ");
    OK.setButtonText(1024, "ДА");
    OK.setButtonText(4194304, "НЕТ");
    if (OK.exec()==QMessageBox::Ok) return true;
    else return false;
}
