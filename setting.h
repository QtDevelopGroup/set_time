#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QDebug>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    ~Settings();
    QFile file;

    QString string,temp1,temp2;
    bool setValue(const QString& ,int);
    int getValue(const QString&,int);

};

#endif // SETTINGS_H
