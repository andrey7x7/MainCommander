#ifndef DIRPANEL_H
#define DIRPANEL_H

#include <QWidget>

namespace Ui {
class DirPanel;
}

class DirPanel : public QWidget
{
    Q_OBJECT

public:
    explicit DirPanel(QWidget *parent = 0);
    ~DirPanel();

private:
    Ui::DirPanel *ui;
    QString dir_loc,dir_dst;
    QString name;

public slots:
    void setDirLoc(QString dir){this->dir_loc=dir;file_in_dir();};
    void setDirDst(QString dir){this->dir_dst=dir;};
    void file_in_dir();
    void on_mkdir();
    void on_copy();
    void on_remove();
    void on_move();
private slots:
    QString up_dir(QString adres);
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_listWidget_itemSelectionChanged();

    bool OK_cancel(QString mesaga);
signals:
    void change_dir(QString dir);
    void open_file(QString file);
    void update_dir();
    void select_dir_file(QString dir_file);
};

#endif // DIRPANEL_H
