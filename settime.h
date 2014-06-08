#ifndef SETTIME_H
#define SETTIME_H

#include <QObject>
#include <windows.h>
#include "setting.h"
class SetTime : public QObject
{
    Q_OBJECT
public:
    explicit SetTime(QObject *parent = 0);
    SYSTEMTIME *st,*backups1,*backups2,*temp_time;
    Settings *setting;
    ~SetTime();
private:
    SYSTEMTIME* time_addition(const SYSTEMTIME* time1,const SYSTEMTIME* time2);
    SYSTEMTIME* time_subduction(const SYSTEMTIME* time1,const SYSTEMTIME* time2);
    int yearToDay(int year,int mon,int day);//������һ������һ�����һ��
    int hourToS(int hour,int min,int s);//�������ʱ������һ��Ķ�����
signals:
    
public slots:
    bool set_system_time(int year,int mon,int day,int hour,int minute,int second);//
    bool recover_system_time();
};

#endif // SETTIME_H
