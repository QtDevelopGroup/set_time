#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QIntValidator>
#include <QString>
#include "setting.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setting=new Settings(this);
    ui->setupUi(this);
    validator_year= new QIntValidator(1000,9999,this->ui->year);
    validator_mon= new QIntValidator(1,12,this->ui->mon);
    validator_day= new QIntValidator(1,31,this->ui->day);
    validator_hour= new QIntValidator(0,23,this->ui->hour);
    validator_min= new QIntValidator(0,59,this->ui->min);
    validator_s= new QIntValidator(0,59,this->ui->second);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label0->setAlignment(Qt::AlignCenter);
    ui->go->setDefault(true);
    ui->re->setEnabled(setting->getValue("re_switch",0));
    ui->year->setValidator(validator_year);
    ui->mon->setValidator(validator_mon);
    ui->day->setValidator(validator_day);
    ui->hour->setValidator(validator_hour);
    ui->min->setValidator(validator_min);
    ui->second->setValidator(validator_s);
    QString string;
    ui->year->setText(string.setNum(setting->getValue("year",2013)));
    ui->mon->setText(string.setNum(setting->getValue("mon",1)));
    ui->day->setText(string.setNum(setting->getValue("day",1)));
    ui->hour->setText(string.setNum(setting->getValue("hour",0)));
    ui->min->setText(string.setNum(setting->getValue("min",0)));
    ui->second->setText(string.setNum(setting->getValue("sencond",0)));

    connect(this->ui->go,SIGNAL(clicked()),this,SLOT(set_time()));
    connect(this->ui->re,SIGNAL(clicked()),this,SLOT(re_time()));
}

void MainWindow::set_time()
{
    int year=this->ui->year->text().toInt();
    int mon=this->ui->mon->text().toInt();
    int day=this->ui->day->text().toInt();
    int hour=this->ui->hour->text().toInt();
    int min=this->ui->min->text().toInt();
    int s=this->ui->second->text().toInt();
    //qDebug()<<year<<" "<<mon<<" "<<day<<" "<<hour<<" "<<min<<" "<<s;
    setting->setValue("year",year);
    setting->setValue("mon",mon);
    setting->setValue("day",day);
    setting->setValue("hour",hour);
    setting->setValue("min",min);
    setting->setValue("second",s);
    //qDebug()<<setting->getValue("year",2013);
    if((year%4==0&&year%100!=0)||year%400==0)
    {
        if(mon==2&&day>29)
        {
            this->ui->day->setText("29");
            day=29;
        }
    }
    else if(mon==2&&day>28)
    {
        this->ui->day->setText("28");
        day=28;
    }
    if(day>30&&(mon==4||mon==6||mon==9||mon==11))
    {
        this->ui->day->setText("30");
        day=30;
    }
    if(hour<8)
    {
        if(day==1)
        {
            day=31;
            if(mon==1)
            {
                mon=12;
                year-=1;
            }
            else{
                mon-=1;
                if((year%4==0&&year%100!=0)||year%400==0)
                {
                    if(mon==2&&day>29)
                        day=29;
                }
                else if(mon==2&&day>28)
                    day=28;
                if(day>30&&(mon==4||mon==6||mon==9||mon==11))
                    day=30;
            }
        }
        else day-=1;
        hour+=16;
    }
    else hour-=8;

    //qDebug()<<"hour:"<<hour;
    if(!time.set_system_time(year,mon,day,hour,min,s))
        this->ui->label->setText(tr("应用失败"));
    else {
        this->ui->label->setText(tr("应用完成"));
        setting->setValue("re_switch",1);
        this->ui->re->setEnabled(true);
    }
    this->ui->label->setVisible(true);
        //qDebug()<<"set error";
}
void MainWindow::re_time()
{
    if(!time.recover_system_time())
        this->ui->label->setText(tr("撤销失败"));
    else {
        this->ui->label->setText(tr("撤销完成"));
        this->ui->re->setEnabled(false);
        setting->setValue("re_switch",0);
    }
    this->ui->label->setVisible(true);
        //qDebug()<<"re error";
}

MainWindow::~MainWindow()
{
    delete ui;
    delete validator_year;
    delete validator_mon;
    delete validator_day;
    delete validator_hour;
    delete validator_min;
    delete validator_s;
    setting->deleteLater();
}
