#include "setting.h"
#include <QTextIStream>
#include <QString>
#include <QDir>
Settings::Settings(QObject *parent) :
    QObject(parent)
{
    temp1="C:/setTime/";
    temp2=".txt";
    QDir dir(temp1);
    if(!dir.exists())
        dir.mkdir(temp1);
}
Settings::~Settings ()
{

}

bool Settings::setValue(const QString &ch, int n)
{
    string=QString("%1%2%3").arg(temp1).arg(ch).arg(temp2);
    file.setFileName(string);
    QTextStream text(&file);
    //qDebug()<<"file name:"<<file.fileName();
    if(file.open (QIODevice::ReadWrite))
    {
        text<<n<<"       ";
        //qDebug()<<ch<<":"<<n;
        file.close();
    }
    else{
        qDebug()<<"errors";
        return false;
    }
    return true;
}
int Settings::getValue(const QString &ch, int n)
{
    string=QString("%1%2%3").arg(temp1).arg(ch).arg(temp2);
    file.setFileName(string);
    QTextStream text(&file);
    if(file.exists())
    {
        //qDebug()<<"haha";
        if(file.open (QIODevice::ReadOnly))
        {
            int temp=text.readAll().toInt();
            file.close();
            return temp;
        }
    }
    return n;
}
