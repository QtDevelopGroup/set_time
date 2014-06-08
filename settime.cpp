#include "settime.h"
#include <QDebug>
#include <QDateTime>
#include "setting.h"
SetTime::SetTime(QObject *parent) :
    QObject(parent)
{
    setting=new Settings(this);
    st=new SYSTEMTIME;
    backups1=new SYSTEMTIME;
    backups2=new SYSTEMTIME;
    temp_time=new SYSTEMTIME;
}
bool SetTime::set_system_time(int year,int mon,int day,int hour,int minute,int second)//
{
    GetSystemTime(backups1);
    //qDebug()<<year<<" "<<mon<<" "<<day<<" "<<hour<<" "<<minute<<" "<<second;
    //qDebug()<<backups1->wMonth<<" "<<backups1->wDay<<" "<<backups1->wHour<<" "<<backups1->wMinute<<" "<<backups1->wSecond;
    //qDebug()<<QDateTime::currentDateTime();
    st->wYear=year;//year;
    st->wMonth=mon;//mon;
    st->wDay=day;//day;
    st->wHour=hour;//hour;
    st->wMinute=minute;//minute;
    st->wSecond=second;//second

    //qDebug()<<st->wYear<<" "<<st->wMonth<<" "<<st->wDay<<" "<<st->wHour<<" "<<st->wMinute<<" "<<st->wSecond;
    if(!SetSystemTime(st))
        return false;
    else{
        GetSystemTime(backups2);
        qDebug()<<"back2:"<<backups2->wMonth<<backups2->wDay<<backups2->wHour<<backups2->wMinute<<backups2->wSecond;
        setting->setValue("back1_year",backups1->wYear);
        setting->setValue("back1_mon",backups1->wMonth);
        setting->setValue("back1_day",backups1->wDay);
        setting->setValue("back1_hour",backups1->wHour);
        setting->setValue("back1_min",backups1->wMinute);
        setting->setValue("back1_second",backups1->wSecond);
        setting->setValue("back2_year",backups2->wYear);
        setting->setValue("back2_mon",backups2->wMonth);
        setting->setValue("back2_day",backups2->wDay);
        setting->setValue("back2_hour",backups2->wHour);
        setting->setValue("back2_min",backups2->wMinute);
        setting->setValue("back2_second",backups2->wSecond);
        return true;
    }
}
bool SetTime::recover_system_time()
{
    backups1->wYear=setting->getValue("back1_year",2013);
    backups1->wMonth=setting->getValue("back1_mon",1);
    backups1->wDay=setting->getValue("back1_day",1);
    backups1->wHour=setting->getValue("back1_hour",0);
    backups1->wMinute=setting->getValue("back1_min",0);
    backups1->wSecond=setting->getValue("back1_second",0);
    backups2->wYear=setting->getValue("back2_year",2013);
    backups2->wMonth=setting->getValue("back2_mon",1);
    backups2->wDay=setting->getValue("back2_day",1);
    backups2->wHour=setting->getValue("back2_hour",0);
    backups2->wMinute=setting->getValue("back2_min",0);
    backups2->wSecond=setting->getValue("back2_second",0);
    qDebug()<<"back1:"<<backups1->wMonth<<backups1->wDay<<backups1->wHour<<backups1->wMinute<<backups1->wSecond;
    qDebug()<<"back2:"<<backups2->wMonth<<backups2->wDay<<backups2->wHour<<backups2->wMinute<<backups2->wSecond;
    GetSystemTime(st);
    qDebug()<<"st:"<<st->wMonth<<st->wDay<<st->wHour<<st->wMinute<<st->wSecond;
    st=time_addition(time_subduction(st,backups2),backups1);
    qDebug()<<"st:"<<st->wMonth<<st->wDay<<st->wHour<<st->wMinute<<st->wSecond;
    if(!SetSystemTime(st))
        return false;
    else
        return true;
}
SYSTEMTIME* SetTime::time_addition(const SYSTEMTIME *time1, const SYSTEMTIME *time2)
{
    qDebug()<<"time1:"<<time1->wYear<<time1->wMonth<<time1->wDay<<time1->wHour<<time1->wMinute<<time1->wSecond;
    qDebug()<<"time2:"<<time2->wYear<<time2->wMonth<<time2->wDay<<time2->wHour<<time2->wMinute<<time2->wSecond;
    bool CF=false;
    int year=time1->wYear+time2->wYear;
    int s1=hourToS(time1->wHour,time1->wMinute,time1->wSecond);
    int s2=hourToS(time2->wHour,time2->wMinute,time2->wSecond);
    qDebug()<<"s1:"<<s1<<"s2:"<<s2;
    s1+=s2;
    qDebug()<<"s1:"<<s1;
    if(s1>24*3600)
    {
        CF=true;
        s1-=24*3600;
    }
    temp_time->wHour=s1/3600;
    temp_time->wMinute=(s1%3600)/60;
    temp_time->wSecond=s1%60;
    int m[13]={0};
    m[1]=31;
    m[2]=28+m[1];
    m[3]=31+m[2];
    m[4]=30+m[3];
    m[5]=31+m[4];
    m[6]=30+m[5];
    m[7]=31+m[6];
    m[8]=31+m[7];
    m[9]=30+m[8];
    m[10]=31+m[9];
    m[11]=30+m[10];
    m[12]=31+m[11];
    int day1=yearToDay(time1->wYear,time1->wMonth,time1->wDay);
    int day2=yearToDay(time2->wYear,time2->wMonth,time2->wDay);
    qDebug()<<"day1:"<<day1<<"day2:"<<day2;
    day1+=day2;
    qDebug()<<"day1:"<<day1;
    if(CF) day1+=1;
    qDebug()<<"day1:"<<day1;
    if((year%4==0&&year%100!=0)||year%400==0)//如果是闰年
    {
        if(day1>366)
        {
            year+=1;
            day1-=366;
        }
    }
    else if(day1>365)
    {
        year+=1;
        day1-=365;
    }
    qDebug()<<"year:"<<year;
    qDebug()<<"day1:"<<day1;
    temp_time->wYear=year;
    if((year%4==0&&year%100!=0)||year%400==0)//如果是闰年
        m[3]+=1;
    for(int i=11;i>=0;i--)
        if(day1>m[i])
        {
            temp_time->wMonth=i+1;
            temp_time->wDay=day1-m[i];
            break;
        }
    qDebug()<<"temp_time"<<temp_time->wYear<<temp_time->wMonth<<temp_time->wDay<<temp_time->wHour<<temp_time->wMinute<<temp_time->wSecond;
    return temp_time;
}
SYSTEMTIME* SetTime::time_subduction(const SYSTEMTIME *time1, const SYSTEMTIME *time2)
{
    qDebug()<<"time1:"<<time1->wYear<<time1->wMonth<<time1->wDay<<time1->wHour<<time1->wMinute<<time1->wSecond;
    qDebug()<<"time2:"<<time2->wYear<<time2->wMonth<<time2->wDay<<time2->wHour<<time2->wMinute<<time2->wSecond;
    int CF=false;
    int year=time1->wYear-time2->wYear;
    int s1=hourToS(time1->wHour,time1->wMinute,time1->wSecond);
    int s2=hourToS(time2->wHour,time2->wMinute,time2->wSecond);
    qDebug()<<"year:"<<year;
    qDebug()<<"s1:"<<s1<<"s2:"<<s2;
    s1-=s2;
    qDebug()<<"s1:"<<s1;
    if(s1<0)
    {
        CF=true;
        s1+=24*3600;
    }
    qDebug()<<"s1:"<<s1;
    temp_time->wHour=s1/3600;
    temp_time->wMinute=(s1%3600)/60;
    temp_time->wSecond=s1%60;
    int m[13]={0};
    m[1]=31;
    m[2]=28+m[1];
    m[3]=31+m[2];
    m[4]=30+m[3];
    m[5]=31+m[4];
    m[6]=30+m[5];
    m[7]=31+m[6];
    m[8]=31+m[7];
    m[9]=30+m[8];
    m[10]=31+m[9];
    m[11]=30+m[10];
    m[12]=31+m[11];
    int day1=yearToDay(time1->wYear,time1->wMonth,time1->wDay);
    int day2=yearToDay(time2->wYear,time2->wMonth,time2->wDay);
    qDebug()<<"day1:"<<day1<<"day2:"<<day2;
    day1-=day2;
    qDebug()<<"day1:"<<day1;
    if(CF) day1-=1;
    CF=false;
    if(day1<0)
    {
        year-=1;
        if((year%4==0&&year%100!=0)||year%400==0)//如果是闰年
           day1+=356;
        else day1+=355;
    }
    qDebug()<<"day1:"<<day1;
    temp_time->wYear=year;
    if((year%4==0&&year%100!=0)||year%400==0)//如果是闰年
        m[3]+=1;
    for(int i=11;i>=0;i--)
        if(day1>m[i])
        {
            temp_time->wMonth=i+1;
            temp_time->wDay=day1-m[i];
            break;
        }
    qDebug()<<temp_time->wYear<<temp_time->wMonth<<temp_time->wDay<<temp_time->wHour<<temp_time->wMinute<<temp_time->wSecond;
    return temp_time;
}
int SetTime::yearToDay(int year, int mon, int day)
{
    int m[13],sum=0;
    m[1]=31;
    m[2]=28;
    m[3]=31;
    m[4]=30;
    m[5]=31;
    m[6]=30;
    m[7]=31;
    m[8]=31;
    m[9]=30;
    m[10]=31;
    m[11]=30;
    m[12]=31;
    if((year%4==0&&year%100!=0)||year%400==0)//如果是闰年
        m[2]=29;
    for(int i=1;i<mon;i++)
        sum+=m[i];
    sum+=day;
    return sum;
}
int SetTime::hourToS(int hour, int min, int s)
{
    return hour*3600+min*60+s;
}

SetTime::~SetTime()
{
    setting->deleteLater();
    delete st;
    delete backups1;
    delete backups2;
    delete temp_time;
}
