#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QMainWindow>
#include <QFileSystemWatcher>
namespace Ui {
class Notepad;
}

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = 0,QString on_file="");
    ~Notepad();

private slots:
    void open_file();
    void on_actionClose_triggered();

    void on_actionSave_triggered();

    void changed_file(QString txt);
private:
    Ui::Notepad *ui;
    QString on_file;
    QFileSystemWatcher *SysWatch;
};

#endif // NOTEPAD_H
