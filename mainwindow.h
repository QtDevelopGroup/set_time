#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settime.h"
#include <QIntValidator>
#include "setting.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    SetTime time;
    Settings *setting;
    explicit MainWindow(QWidget *parent = 0);
    QIntValidator  *validator_year,*validator_mon,*validator_day,*validator_hour,*validator_min,*validator_s;
    ~MainWindow();
    
public slots:
    void set_time();
    void re_time();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
